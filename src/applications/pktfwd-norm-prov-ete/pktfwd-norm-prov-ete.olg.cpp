/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(linkhr,infinity,infinity,keys(2)). /*Links between hosts and routers*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/
/*materialize(device,infinity,infinity,keys(2:int32)).Identify the device type of the node*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(4:list)).
materialize(prov, infinity, infinity, keys(2:cid,3:cid,4)).




/* Edb provenance rules*/
r00 prov(@Node, VID, RID, Node) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    VID := f_sha1("initPacket" + Node + SrcAdd + DstAdd + Data),
    RID := VID.

/*r01 prov(@Device, VID, RID, Device) :-
    device(@Device, Dvtype),
    VID := f_sha1("device" + Device + Dvtype),
    RID := VID.*/

/*r03 prov(@Node, VID, RID, Node) :-
    flowEntry(@Node, DstEntry, Next),
    VID := f_sha1("Node" + DstEntry + Next),
    RID := VID.
*/

/*Switch program*/
/*A hit in the routing table, forward the packet accordingly*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List) :-
    packet(@Node, SrcAdd, DstAdd, Data),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PID2 := f_sha1(((("packet"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    PID3 := f_sha1((("flowEntry"+ Node)+ DstAdd)+ Next),
    List3 := f_append(PID3),
    List := f_concat(List, List3),
    PID4 := f_sha1(("link"+ Node)+ Next),
    List4 := f_append(PID4),
    List := f_concat(List, List4),
    RLOC := Node,
    R := "rs1",
    RID := f_sha1((R+ RLOC)+ List).

prov_rs1_2 ruleExec(@RLOC, RID, R, List) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List).

prov_rs1_3 epacket(@Next, SrcAdd, DstAdd, Data, RID, RLOC) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List).

prov_rs1_4 packet(@Next, SrcAdd, DstAdd, Data) :-
    epacket(@Next, SrcAdd, DstAdd, Data, RID, RLOC).

prov_rs1_5 prov(@Next, VID, RID, RLOC) :-
    epacket(@Next, SrcAdd, DstAdd, Data, RID, RLOC),
    VID := f_sha1(((("packet"+ Next)+ SrcAdd)+ DstAdd)+ Data).

/*Host program*/
prov_rh1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PID2 := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    PID3 := f_sha1(("link"+ Node)+ Next),
    List3 := f_append(PID3),
    List := f_concat(List, List3),
    RLOC := Node,
    R := "rh1",
    RID := f_sha1((R+ RLOC)+ List).

/*Receive a packet*/
prov_rh2_1 erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List) :-
    packet(@Node, SrcAdd, DstAdd, Data),
    DstAdd == Node,
    PID2 := f_sha1(((("packet"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    RLOC := Node,
    R := "rh2",
    RID := f_sha1((R+ RLOC)+ List).

prov_rh2_2 ruleExec(@RLOC, RID, R, List) :-
    erecvPacketTemp(@RLOC,Node, SrcAdd, DstAdd, Data, RID, R, List).

prov_rh2_3 erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC) :-
    erecvPacketTemp(@RLOC,Node, SrcAdd, DstAdd, Data, RID, R, List).

prov_rh2_4 recvPacket(@Node, SrcAdd, DstAdd, Data) :-
    erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC).

prov_rh2_5 prov(@Node, VID, RID, RLOC) :-
    erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC),
    VID := f_sha1(((("recvPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data).
