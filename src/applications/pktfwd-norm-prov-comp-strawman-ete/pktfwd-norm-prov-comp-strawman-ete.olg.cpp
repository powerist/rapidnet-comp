/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(linkhr,infinity,infinity,keys(2)). /*Links between hosts and routers*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(device,infinity,infinity,keys(2:int32)). /*Identify the device type of the node*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(2:cid)).
materialize(recvAuxPkt, infinity, infinity, keys(2:cid)).




/*Switch program*/
/*A hit in the routing table, forward the packet accordingly*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList) :-
    packet(@Node, SrcAdd, DstAdd, Data, HashList),
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
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList),
    ruleExec(@RLOC, RID, R, List).

prov_rs1_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_rs1_4 packet(@Next, SrcAdd, DstAdd, Data, NewHashList) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList),
    Hash := f_append(RID),
    NewHashList := f_concat(HashList, Hash).

/*Host program*/
prov_rh1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PIDev := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    HashList := f_append(PIDev),
    PID2 := f_sha1(("link"+ Node)+ Next),
    List := f_append(PID2),
    RLOC := Node,
    R := "rh1",
    RID := f_sha1((R+ RLOC)+ List).

/*Receive a packet*/
prov_rh2_1 erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, HashList) :-
    packet(@Node, SrcAdd, DstAdd, Data, HashList),
    DstAdd == Node,
    PID2 := f_sha1(((("packet"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    RLOC := Node,
    R := "rh2",
    RID := f_sha1((R+ RLOC)+ List).

prov_rh2_2 eRecvPacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, HashList),
    ruleExec(@RLOC, RID, R, List).

prov_rh2_3 ruleExec(@RLOC, RID, R, List) :-
    eRecvPacketCount(@RLOC, RID, R, List, Count),
    Count == 0.

prov_rh2_4 erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC, HashList) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, HashList),
    Hash := f_append(RID),
    NewHashList := f_concat(HashList, Hash).

prov_rh2_5 recvPacket(@Node, SrcAdd, DstAdd, Data) :-
    erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC, HashList).

prov_rh2_6 recvAuxPkt(@Node, PID, HashList) :-
    erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC, HashList),
    PID := f_sha1("recvPacket" + Node + SrcAdd + DstAdd + Data).
