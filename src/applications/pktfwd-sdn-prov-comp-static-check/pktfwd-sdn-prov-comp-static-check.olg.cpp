/* initPacket is the event tuple */
/* Note: we remove RLoc from RID in this version*/

/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(linkhr,infinity,infinity,keys(2)). /*Links between hosts and routers*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2,4:int32)). /*Flow tables of switches*/
materialize(maxPriority,infinity,infinity,keys(2:int32)). /*Determine the maximum priority of the flow table*/
materialize(device,infinity,infinity,keys(2:int32)). /*Identify the device type of the node*/
materialize(equiHashTable,infinity,infinity,keys(2)). /*Hash table for equivalence detection*/
materialize(provHashTable,infinity,infinity,keys(3:list)). /*Hash table for provenance query*/
materialize(ruleExec,infinity,infinity,keys(3:str, 4:list)). /*Hash table for provenance query*/

/* Provenance tables*/
materialize(ruleExec,infinity,infinity,keys(4:list)). /*Record the rule execution*/
materialize(recvAuxPkt,infinity,infinity,keys(2,3:str)). /*Record the extra hash values*/




/*Switch program*/
/* Rules for handling provenance*/
rs10 eMatchingPacketTemp(@RLoc, Switch, SrcAdd, DstAdd, TopPriority, RID, R, List, DataTag) :-
    device(@Switch, Dvtype),
    packetProv(@Switch, SrcAdd, DstAdd, Data, Tag),
    maxPriority(@Switch, TopPriority),
    Dvtype == 0,
    RLoc := Switch,
    R := "rs1",
    PID1 := f_sha1("device" + Switch + Dvtype),
    PID2 := f_sha1("maxPriority" + Switch + TopPriority),
    List := f_append(PID1),
    List2 := f_append(PID2),
    List := f_concat(List, List2),
    RID := f_sha1(R + List),
    DataList := f_append(Data),
    DataTag := f_concat(DataList, Tag).

rs11 eMatchingPacket(@Switch, SrcAdd, DstAdd, Data, TopPriority, RID, RLoc, Tag) :-
    eMatchingPacketTemp(@RLoc, Switch, SrcAdd, DstAdd, TopPriority, RID, R, List, DataTag),
    Data := f_first(DataTag),
    Tag := f_removeFirst(DataTag).

/* TODO: The performance of using aggregation for hash table is not expected to be good */
/* TODO: Matching on all ruleExec attributes is not necessary*/
rs12 eMatchingPacketCount(@RLoc, RID, R, List, a_COUNT<*>) :-
    eMatchingPacketTemp(@RLoc, Switch, SrcAdd, DstAdd, TopPriority, RID, R, List, DataTag),
    ruleExec(@RLoc, RID, R, List).

/* If the RID does not exist, store the provenance node*/
rs13 ruleExec(@RLoc, RID, R, List) :-
    eMatchingPacketCount(@RLoc, RID, R, List, Rcount),
    Rcount == 0.

/* Add the RID to the Hashlist*/
rs14 matchingPacket(@Switch, SrcAdd, DstAdd, Data, TopPriority, NewTag) :-
    eMatchingPacket(@Switch, SrcAdd, DstAdd, Data, TopPriority, RID, RLoc, Tag),
    Hash := f_append(RID),
    NewTag := f_concat(Tag, Hash).

/* Rules for normal execution without provenance */
rs1 matchingPacketNonProv(@Switch, SrcAdd, DstAdd, Data, TopPriority, PIDequi, PIDev) :-
        device(@Switch, Dvtype),
 packetNonProv(@Switch, SrcAdd, DstAdd, Data, PIDequi, PIDev),
 maxPriority(@Switch, TopPriority),
        Dvtype == 0 .

/*Recursively matching entries in a routing table*/
rs20 eMatchingPacketTemp(@RLoc, Switch, SrcAdd, DstAdd, NextPriority, RID, R, List, DataTag) :-
    device(@Switch, Dvtype),
    matchingPacket(@Switch, SrcAdd, DstAdd, Data, Priority, Tag),
    flowEntry(@Switch, DstEntry, Next, Priority),
    Priority > 0,
    DstAdd != DstEntry,
    NextPriority := Priority - 1,
    Dvtype == 0,
    RLoc := Switch,
    R := "rs2",
    PID1 := f_sha1("device" + Switch + Dvtype),
    PID2 := f_sha1("flowEntry" + Switch + DstEntry + Next + Priority),
    List := f_append(PID1),
    List2 := f_append(PID2),
    List := f_concat(List, List2),
    RID := f_sha1(R + List),
    DataList := f_append(Data),
    DataTag := f_concat(DataList, Tag).

/* rs21 - rs24 are identical to rs11 - rs14*/

/*Recursively matching entries in a routing table*/
rs2 matchingPacketNonProv(@Switch, SrcAdd, DstAdd, Data, NextPriority, PIDequi, PIDev) :-
        device(@Switch, Dvtype),
 matchingPacketNonProv(@Switch, SrcAdd, DstAdd, Data, Priority, PIDequi, PIDev),
 flowEntry(@Switch, DstEntry, Next, Priority),
 Priority > 0,
 DstAdd != DstEntry,
 NextPriority := Priority - 1,
        Dvtype == 0 .

/*A hit in the routing table, forward the packet accordingly*/
rs30 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
        device(@Switch, Dvtype),
        matchingPacket(@Switch, SrcAdd, DstAdd, Data, Priority, Tag),
 flowEntry(@Switch, DstEntry, Next, Priority),
 link(@Switch, Next),
 Priority > 0,
 DstAdd == DstEntry,
        Dvtype == 0,
 RLoc := Switch,
 R := "rs3",
 PID1 := f_sha1("device" + Switch + Dvtype),
 PID2 := f_sha1("flowEntry" + Switch + DstEntry + Next + Priority),
 PID3 := f_sha1("link" + Switch + Next),
 List := f_append(PID1),
 List2 := f_append(PID2),
 List := f_concat(List, List2),
 List3 := f_append(PID3),
 List := f_concat(List, List3),
 RID := f_sha1(R + List).

rs31 ePacket(@Next, SrcAdd, DstAdd, Data, RID, RLoc, Tag) :-
 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag).

/*TODO: optimization - join on RID is enough*/
rs32 ePaketCount(@RLoc, RID, R, List, a_COUNT<*>) :-
 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
 ruleExec(@RLoc, RID, R, List).

rs33 ruleExec(@RLoc, RID, R, List) :-
 ePaketCount(@RLoc, RID, R, List, Rcount),
 Rcount == 0.

/* Since Hashlist is the suffix of Tag */
/* appending Hash to Tag is equal to appending Hash to Hashlist*/
rs34 packetProv(@Next, SrcAdd, DstAdd, Data, NewTag) :-
 ePacket(@Next, SrcAdd, DstAdd, Data, RID, RLoc, Tag),
 Hash := f_append(RID),
 NewTag := f_concat(Tag, Hash).

/*A hit in the routing table, forward the packet accordingly*/
rs3 packetNonProv(@Next, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
        device(@Switch, Dvtype),
        matchingPacketNonProv(@Switch, SrcAdd, DstAdd, Data, Priority, PIDequi, PIDev),
 flowEntry(@Switch, DstEntry, Next, Priority),
 link(@Switch, Next),
 Priority > 0,
 DstAdd == DstEntry,
        Dvtype == 0 .


/*No hit in the routing table, follow the default routing*/
rs40 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
        device(@Switch, Dvtype),
        matchingPacket(@Switch, SrcAdd, DstAdd, Data, Priority, Tag),
 flowEntry(@Switch, DstAdd, Next, Priority),
 link(@Switch, Next),
 Priority == 0,
        Dvtype == 0,
 RLoc := Switch,
 R := "rs4",
 PID1 := f_sha1("device" + Switch + Dvtype),
 PID2 := f_sha1("flowEntry" + Switch + DstAdd + Next + Priority),
 PID3 := f_sha1("link" + Switch + Next),
 List := f_append(PID1),
 List2 := f_append(PID2),
 List := f_concat(List, List2),
 List3 := f_append(PID3),
 List := f_concat(List, List3),
 RID := f_sha1(R + List).

/* rs41 - rs44 are identical to rs31 - rs34*/

/*No hit in the routing table, follow the default routing*/
rs4 packetNonProv(@Next, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
        device(@Switch, Dvtype),
        matchingPacketNonProv(@Switch, SrcAdd, DstAdd, Data, Priority, PIDequi, PIDev),
 flowEntry(@Switch, DstAdd, Next, Priority),
 link(@Switch, Next),
 Priority == 0,
        Dvtype == 0 .

/*Host program*/
/* Hash on equivalence attributes*/
rh100 initPacketHash(@Host, SrcAdd, DstAdd, Data, PIDequi) :-
    initPacket(@Host, SrcAdd, DstAdd, Data),
    PIDequi := f_sha1("" + Host + DstAdd).

rh101 initPacketCount(@Host, SrcAdd, DstAdd, Data, PIDequi, a_COUNT<*>) :-
    initPacketHash(@Host, SrcAdd, DstAdd, Data, PIDequi),
    equiHashTable(@Host, DstAdd, PIDequi).

rh102 equiHashTable(@Host, DstAdd, PIDequi) :-
    initPacketCount(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDcount),
    PIDcount == 0.

rh103 packetNonProv(@Switch, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
    device(@Host, Dvtype),
    initPacketCount(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDcount),
    linkhr(@Host, Switch),
    Dvtype == 1,
    PIDev := f_sha1("initPacket" + Host + SrcAdd + DstAdd + Data), /*Generate event hash*/
    PIDcount != 0.

rh104 ePacketTemp(@RLoc, Switch, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
        device(@Host, Dvtype),
        initPacketCount(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDcount),
 linkhr(@Host, Switch),
        Dvtype == 1,
 PIDcount == 0,
 RLoc := Host,
 R := "rh1",
 PIDev := f_sha1("initPacket" + Host + SrcAdd + DstAdd + Data), /*Generate event hash*/
 Equilist := f_append(PIDequi),
 Evlist := f_append(PIDev),
 Tag := f_concat(Equilist, Evlist),
 PID1 := f_sha1("device" + Host + Dvtype), /*Generate rule ID*/
 PID2 := f_sha1("linkhr" + Host + Switch),
 List := f_append(PID1),
 List2 := f_append(PID2),
 List := f_concat(List, List2),
 RID := f_sha1(R + List).

/* rh11 - rh14 are identical to rs31 - rs34*/

/*Receive a packet*/
/* Process a packet that is the first of the equivalence class*/
rh20 eRecvPacketTemp(@RLoc, Host, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
 device(@Host, Dvtype),
 packetProv(@Host, SrcAdd, DstAdd, Data, Tag),
 DstAdd == Host,
        Dvtype == 1,
 RLoc := Host,
 R := "rh2",
 PID1 := f_sha1("device" + Host + Dvtype),
 List := f_append(PID1),
 RID := f_sha1(R + List).

rh21 eRecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLoc, Tag) :-
    eRecvPacketTemp(@RLoc, Host, SrcAdd, DstAdd, Data, RID, R, List, Tag).

rh22 eRecvPacketCount(@RLoc, RID, R, List, a_COUNT<*>) :-
 eRecvPacketTemp(@RLoc, Host, SrcAdd, DstAdd, Data, RID, R, List, Tag),
 ruleExec(@RLoc, RID, R, List).

rs23 ruleExec(@RLoc, RID, R, List) :-
 eRecvPacketCount(@RLoc, RID, R, List, Rcount),
 Rcount == 0.

rh24 recvPacketTemp(@Host, SrcAdd, DstAdd, Data, Tag) :-
 eRecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLoc, Tag),
 Hash := f_append(RID),
 NewTag := f_concat(Tag, Hash).

rh25 recvPacketDecomp(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev, Hashlist) :-
    recvPacketTemp(@Host, SrcAdd, DstAdd, Data, Tag),
    PIDequi := f_first(Tag),
    Remainlist := f_removeFirst(Tag),
    PIDev := f_first(Remainlist),
    Hashlist := f_removeFirst(Remainlist).

rh26 provHashTable(@Host, PIDequi, Hashlist) :-
    recvPacketDecomp(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev, Hashlist).

rh27 recvPacket(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
    recvPacketDecomp(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev, Hashlist).

/* Process a packet that is NOT the first of the equivalence class*/
rh2 recvPacket(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
        device(@Host, Dvtype),
 packetNonProv(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev),
        Dvtype == 1 .

rh28 recvAuxPkt(@Host, SrcAdd, Data, PIDequi, PIDev) :-
        recvPacket(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev).
