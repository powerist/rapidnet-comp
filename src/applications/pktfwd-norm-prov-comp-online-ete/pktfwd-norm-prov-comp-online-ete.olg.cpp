/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(linkhr,infinity,infinity,keys(2)). /*Links between hosts and routers*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(device,infinity,infinity,keys(2:int32)). /*Identify the device type of the node*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(4:list)).
materialize(equiHashTable,infinity,infinity,keys(2)). /*Hash table for equivalence detection*/
materialize(provHashTable,infinity,infinity,keys(3:list)). /*Hash table for provenance query*/
materialize(recvAuxPkt, infinity, infinity, keys(2:cid)).

/* 'ID' is a keyword. Parse error on using id for type!
The workaround is to call it cid or chord id. */




/*Switch program*/
/*A hit in the routing table, forward the packet accordingly*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    packetProv(@Node, SrcAdd, DstAdd, Data, Tag),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PID3 := f_sha1((("flowEntry"+ Node)+ DstAdd)+ Next),
    List := f_append(PID3),
    PID4 := f_sha1(("link"+ Node)+ Next),
    List4 := f_append(PID4),
    List := f_concat(List, List4),
    RLOC := Node,
    R := "rs1",
    RID := f_sha1((R+ RLOC)+ List).

prov_rs1_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ruleExec(@RLOC, RID, R, List).

prov_rs1_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_rs1_4 packetProv(@Next, SrcAdd, DstAdd, Data, NewTag) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    Hash := f_append(RID),
    NewTag := f_concat(Tag, Hash).

/* Rules for normal execution without provenance */

rs1 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
        packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 flowEntry(@Node, DstAdd, Next),
 link(@Node, Next).

/*Host program*/
/* Hash on equivalence attributes*/
rh100 initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    PIDequi := f_sha1("" + Node + DstAdd).

rh101 initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, a_COUNT<*>) :-
    initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi),
    equiHashTable(@Node, DstAdd, PIDequi).

rh102 equiHashTable(@Node, DstAdd, PIDequi) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDcount),
    PIDcount == 0.

rh103 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDcount),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PIDev := f_sha1("initPacket" + Node + SrcAdd + DstAdd + Data),
    PIDcount != 0,
    PIDequiHash := f_append(PIDequi),
    PIDevHash := f_append(PIDev),
    PIDHash := f_concat(PIDequiHash, PIDevHash).

prov_rh1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDcount),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PIDcount == 0,
    PIDev := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    Equilist := f_append(PIDequi),
           Evlist := f_append(PIDev),
           Tag := f_concat(Equilist, Evlist),
    PID2 := f_sha1(("link"+ Node)+ Next),
    List := f_append(PID2),
    RLOC := Node,
    R := "rh1",
    RID := f_sha1((R+ RLOC)+ List).

/*Receive a packet*/
prov_rh2_1 erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    packetProv(@Node, SrcAdd, DstAdd, Data, Tag),
    DstAdd == Node,
    PID2 := f_sha1(((("packet"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    RLOC := Node,
    R := "rh2",
    RID := f_sha1((R+ RLOC)+ List).

prov_rh2_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ruleExec(@RLOC, RID, R, List).

prov_rh2_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_rh2_4 erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC, Tag) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag).

prov_rh2_5 recvPacketTemp(@Node, SrcAdd, DstAdd, Data, Tag) :-
    erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC, Tag),
           Hash := f_append(RID),
           NewTag := f_concat(Tag, Hash).

prov_rh2_6 recvPacketDecomp(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDev, Hashlist) :-
    recvPacketTemp(@Node, SrcAdd, DstAdd, Data, Tag),
    PIDequi := f_first(Tag),
    Remainlist := f_removeFirst(Tag),
    PIDev := f_first(Remainlist),
    Hashlist := f_removeFirst(Remainlist).

prov_rh2_7 provHashTable(@Node, PIDequi, Hashlist) :-
    recvPacketDecomp(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDev, Hashlist).

prov_rh2_8 recvPacket(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
    recvPacketDecomp(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDev, Hashlist).

/*
rh2 recvPacket(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
	packetNonProv(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDev),
	DstAdd == Host.
*/
rh2 recvPacket(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
 packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 PIDequi := f_first(PIDHash),
 PIDevHash := f_removeFirst(PIDHash),
 PIDev := f_first(PIDevHash),
 DstAdd == Node.

rh3 recvAuxPkt(@Node, PID, PIDequi, PIDev) :-
        recvPacket(@Node, SrcAdd, DstAdd, Data, PIDequi, PIDev),
        PID := f_sha1("recvPacket" + Node + SrcAdd + DstAdd + Data).
