/****************
*
* Online compression with distributed hash list
*
*****************/

/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(link,infinity,infinity,keys(2)). /*Links between nodes*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(programID,infinity,infinity,keys(2)). /*Allow the user to specify program ID*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(2:cid)).
materialize(provLink, infinity, infinity, keys(2:cid,3:int32,4,5:cid,6:int32)).
materialize(equiHashTable,infinity,infinity,keys(2)). /*Hash table for equivalence detection*/
materialize(provHashTable,infinity,infinity,keys(3:list)). /*Hash table for provenance query*/
materialize(recvPacketProv, infinity, infinity, keys(2:cid))./*Provenance-queryable recv packet*/

/* Tables for provenance query*/
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(pQList,infinity,infinity,keys(1,2:cid)).
materialize(rQList,infinity,infinity,keys(1,2:cid)).
materialize(pResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).


/* 'ID' is a keyword. Parse error on using id for type!
The workaround is to call it cid or chord id. */




/*Switch program*/
/* Execution of rs1 with provenance enabled*/
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

/* Update provLink, which associates the current RID with the RID of the previous rule.*/
prov_rs1_4 provLink(@RLOC, RID, CurCount, Preloc, PreRID, PreCount) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    PreCount := f_first(Tag),
    CurCount := PreCount + 1,
    TempTag1 := f_removeFirst(Tag),
    Preloc := f_first(TempTag),
    TempTag2 := f_removeFirst(TempTag1),
    PreRID := f_first(TempTag2).

prov_rs1_5 packetProv(@Next, SrcAdd, DstAdd, Data, NewTag) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    Count := f_first(Tag),
    NewCount := Count + 1,
    NewCountlist := f_append(NewCount),
    TempNewTag1 := f_removeFirst(Tag),
    TempNewTag2 := f_removeFirst(TempNewTag1),
    TempNewTag3 := f_removeFirst(TempNewTag2),
    RIDlist := f_append(RID),
    Loclist := f_append(RLOC),
    TempNewTag4 := f_concat(RIDlist, TempNewTag3),
    TempNewTag5 := f_concat(Loclist, TempNewTag4),
    NewTag := f_concat(NewCountlist, TempNewTag5).

/* Execution for original rs1*/
rs1 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
        packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 flowEntry(@Node, DstAdd, Next),
 link(@Node, Next).

/*Host program*/
/* prov_ri1 to prov_ri3 hashes on equivalence-class identifier,*/
/* and check whether the hash value "PIDequi" exists in the hash set "equiHashTable"*/
prov_ri1 initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    programID(@Node, DstAdd, ProgID),
    PIDequi := f_sha1("" + Node + DstAdd).

prov_ri2 initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, a_COUNT<*>) :-
    initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID),
    equiHashTable(@Node, DstAdd, PIDequi).

prov_ri3 equiHashTable(@Node, DstAdd, PIDequi) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    PIDcount == 0.

/* Execution of original rh1*/
rh1 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    link(@Node, Next),
    PIDev := f_sha1("initPacket" + Node + SrcAdd + DstAdd + Data),
    PIDcount != 0,
    PIDequiHash := f_append(PIDequi),
    PIDevHash := f_append(PIDev),
    ProgIDlist := f_append(ProgID),
    PIDHash1 := f_concat(PIDequiHash, PIDevHash),
    PIDHash := f_concat(PIDHash1, ProgIDlist).

/* Execution of rh1 with provenance enabled*/
prov_rh1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    link(@Node, Next),
    PIDcount == 0,
    PID := f_sha1(("link"+ Node)+ Next),
    List := f_append(PID),
    RLOC := Node,
    RID := f_sha1((R+ RLOC)+ List),
    R := "rh1",
    Equilist := f_append(PIDequi),
    PIDev := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
           Evlist := f_append(PIDev),
    ProgIDlist := f_append(ProgID),
    Count := 0,
    Countlist := f_append(Count),
    InitProvNode := f_sha1("NULL"),
    InitProvNodelist := f_append(InitProvNode),
    InitProvHash := f_sha1("NULL"),
    InitProvHashlist := f_append(InitProvHash),
    TempTag1 := f_concat(Countlist, InitProvNodelist),
    TempTag2 := f_concat(TempTag1, InitProvHashlist),
    TempTag3 := f_concat(TempTag2, Equilist),
           TempTag4 := f_concat(TempTag3, Evlist),
    Tag := f_concat(TempTag4, ProgIDlist).

/* Receive a packet with provenance maintenance enabled*/
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

prov_rh2_4 provLink(@RLOC, RID, CurCount, Preloc, PreRID, PreCount) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    PreCount := f_first(Tag),
    CurCount := PreCount + 1,
    TempTag1 := f_removeFirst(Tag),
    Preloc := f_first(TempTag),
    TempTag2 := f_removeFirst(TempTag1),
    PreRID := f_first(TempTag2).

prov_rh2_5 recvPacket(@Node, SrcAdd, DstAdd, Data, NewTag) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    Count := f_first(Tag),
    NewCount := Count + 1,
    NewCountlist := f_append(NewCount),
    TempNewTag1 := f_removeFirst(Tag),
    TempNewTag2 := f_removeFirst(TempNewTag1),
    TempNewTag3 := f_removeFirst(TempNewTag2),
    RIDlist := f_append(RID),
    Loclist := f_append(RLOC),
    TempNewTag4 := f_concat(RIDlist, TempNewTag3),
    TempNewTag5 := f_concat(Loclist, TempNewTag4),
    NewTag := f_concat(NewCountlist, TempNewTag5).


/* Handle provenance information for the output recv tuple*/
/* Create a tuple in provHashTable. The tuple connects the output tuple to a provenance tree*/
ro1 provHashTable(@Node, EquiHash, ProgID, Loc, RID, Count) :-
    recvPacket(@Node, SrcAdd, DstAdd, Data, Tag),
    Count := f_first(Tag),
    RemainTag1 := f_removeFirst(Tag),
    Loc := f_first(RemainTag1),
    RemainTag2 := f_removeFirst(RemainTag1),
    RID := f_first(RemainTag2),
    RemainTag3 := f_removeFirst(RemainTag2),
    PIDequi := f_first(RemainTag3),
    RemainTag4 := f_removeFirst(RemainTag3),
    PIDev := f_first(RemainTag4),
    RemainTag5 := f_removeFirst(RemainTag4),
    ProgID := f_first(RemainTag5),
    EquiHash := f_sha1(ProgID, PIDequi).

ro2 recvPacketProv(@Node, PID, EquiHash, PIDev) :-
    recvPacket(@Node, SrcAdd, DstAdd, Data, Tag),
           PID := f_sha1("recvPacket" + Node + SrcAdd + DstAdd + Data),
    Count := f_first(Tag),
    RemainTag1 := f_removeFirst(Tag),
    Loc := f_first(RemainTag1),
    RemainTag2 := f_removeFirst(RemainTag1),
    RID := f_first(RemainTag2),
    RemainTag3 := f_removeFirst(RemainTag2),
    PIDequi := f_first(RemainTag3),
    RemainTag4 := f_removeFirst(RemainTag3),
    PIDev := f_first(RemainTag4),
    RemainTag5 := f_removeFirst(RemainTag4),
    ProgID := f_first(RemainTag5),
    EquiHash := f_sha1(ProgID, PIDequi).


/* Execution of original rh2*/
rh2 recvPacketNP(@Node, SrcAdd, DstAdd, Data, PIDHash) :-
 packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 DstAdd == Node.

rho3 recvPacketProv(@Node, PID, EquiHash, PIDev) :-
 recvPacketNP(@Node, SrcAdd, DstAdd, Data, PIDHash),
 DstAdd == Node,
        PID := f_sha1("recvPacket" + Node + SrcAdd + DstAdd + Data),
 PIDequi := f_first(PIDHash),
 PIDHash1 := f_removeFirst(PIDHash),
 PIDev := f_first(PIDHash1),
 PIDHash2 := f_removeFirst(PIDHash1),
 ProgID := f_first(PIDHash2),
 EquiHash := f_sha1(ProgID, PIDequi).


/* Querying */

/* EDB vertex */
edb1 baseReturn(@Ret,QID,PID,Prov) :- baseQuery(@X,QID,PID,Ret), Prov:=f_pEDB(PID,X).

/* Query provenance at the root */

/* Find the shared provenance tree and initiate the query*/
idb1 ruleQuery(@Loc,NQID,RID,X, PIDev) :- recvPacketProv(@X,PID,EquiHash,PIDev),
 provHashTable(@X, EquiHash, ProgID, Loc, RID),
 NQID := f_sha1(""+PID+RID).

/* Prepare for query return*/
idb2 pResultTmp(@X,PID,X,PID,Buf) :-
       recvPacketProv(@X,PID,EquiHash,PIDev), Buf:=f_empty().

/* When the rule exec provenance returns, add the provenance to the list*/
idb7 pResultTmp(@X,QID,Ret,VID,Buf) :- rReturn(@X,NQID,RID,Prov),
       pResultTmp(@X,QID,Ret,VID,Buf1), NQID==f_sha1(""+QID+RID),
       Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

/* Return the whole provenance tree*/
idb9 pReturn(@Ret,QID,VID,Prov) :- pResultTmp(@X,QID,Ret,VID,Buf), Prov:=f_pIDB(Buf,X).

/* Query ruleExec tables */

/* Enumerate all base tuples*/
rv1 rQList(@X,QID,List) :- ruleQuery(@X,QID,RID,Ret,PIDev),
      provLink(@X,RID, CurCount, PreLoc, PreRID, PreCount),
      ruleExec(@X, RID, R, List).

/* Query the child ruleExec entry*/
rv2 ruleQuery(@PreLoc,NQID,PreRID,Ret) :- ruleQuery(@X,QID,RID,Ret,PIDev),
      provLink(@X,RID, CurCount, PreLoc, PreRID, PreCount),
      ruleExec(@X, RID, R, List),
      NQID:=f_sha1(""+QID+PreRID),
      PreRID!=f_sha1("NULL").

/* Reach leaf ruleExec entry. Look for the event tuple*/
rv2 baseQuery(@X,NQID,PIDev,X) :- ruleQuery(@X,QID,RID,Ret,PIDev),
      provLink(@X, RID, CurCount, PreLoc, PreRID, PreCount),
      ruleExec(@X, RID, R, List),
      PreRID==f_sha1("NULL"),
      NQID := f_sha1(""+QID+PIDev).

/* Prepare for provenance return from base tuples*/
rv3 rResultTmp(@X,QID,Ret,RID,Buf) :-
      ruleQuery(@X,QID,RID,Ret,PIDev), Buf:=f_empty().

/* Iteratively query all base tuples*/
rv4 rIterate(@X,QID,N) :- ruleQuery(@X,QID,RID,Ret,PIDev), N:=1.
rv5 rIterate(@X,QID,N) :- rIterate(@X,QID,N1),
      rQList(@X,QID,List), N1<f_size(List), N:=N1+1.

/* Query base tables*/

/* Issue query of provenance of the base tuple*/
rv6 baseQuery(@X,NQID,VID,X) :- rIterate(@X,QID,N),
      rQList(@X,QID,List),
      VID:=f_item(List,N), NQID:=f_sha1(""+QID+VID).

/* Collect provenance of base tuples*/
rv7 rResultTmp(@X,QID,Ret,RID,Buf) :- baseReturn(@X,NQID,VID,Prov),
      rResultTmp(@X,QID,Ret,RID,Buf1), NQID==f_sha1(""+QID+VID),
      Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

/* Collect provenance of child ruleExec entry*/
rv8 rResultTmp(@X,QID,Ret,RID,Buf):- rReturn(@X,QID,RID,Prov),
 rResultTmp(@X,QID,Ret,RID,Buf1), NQID==f_sha1(""+QID+RID),
 Buf2:=f_append(Prov), Buf:=f_concat(Buf2, Buf1).

/* Set a marker eRReturn to inform the completeness of provenance information*/
rv9 eRReturn(@X,QID) :- rResultTmp(@X,QID,Ret,RID,Buf),
      rQList(@X,QID,List), f_size(Buf)==f_size(List)+1.

/* Return the provenance information of the current ruleExec*/
rv10 rReturn(@Ret,QID,RID,Prov) :- eRReturn(@X,QID),
      rResultTmp(@X,QID,Ret,RID,Buf),
      ruleExec(@X,RID,R,List,PreLoc,PreRID), Prov:=f_pRULE(Buf,X,R).
