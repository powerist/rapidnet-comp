/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
/*materialize(recvPacket,infinity,infinity,keys(2,3,4:str)).*/
materialize(linkhr,infinity,infinity,keys(2)). /*Links between hosts and routers*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/

/*Tables from the provenance-query.olg*/
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(pQList,infinity,infinity,keys(1,2:cid)).
materialize(rQList,infinity,infinity,keys(1,2:cid)).
materialize(pResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).


/*Tables for automatic provenance querying*/
materialize(recordSD,infinity,infinity,keys(2,3)).
materialize(initProvQuery,infinity,infinity,keys(2:cid,3:cid,4)).

/*materialize(device,infinity,infinity,keys(2:int32)).Identify the device type of the node*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(4:list)).
materialize(prov, infinity, infinity, keys(2:cid,3:cid,4)).




/* Edb provenance rules*/
r00 prov(@Node, VID, RID, Node) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    VID := f_sha1("initPacket" + Node + SrcAdd + DstAdd + Data),
    RID := VID.

r01 prov(@Node, VID, RID, Node) :-
    link(@Node, Next),
    VID := f_sha1("link" + Node+ Next),
    RID := VID.

r03 prov(@Node, VID, RID, Node) :-
    flowEntry(@Node, DstEntry, Next),
    VID := f_sha1("flowEntry" + Node+ DstEntry + Next),
    RID := VID.

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

/* Querying */

/* EDB vertex */
edb1 pReturn(@Ret,QID,VID,Prov) :- provQuery(@X,QID,VID,Ret),
       prov(@X,VID,RID,RLoc), RID==VID, Prov:=f_pEDB(VID,X).

/* IDB vertex */
idb1 pQList(@X,QID,a_LIST<RID>) :- provQuery(@X,QID,VID,Ret),
       prov(@X,VID,RID,RLoc), RID!=VID.
idb2 pResultTmp(@X,QID,Ret,VID,Buf) :-
       provQuery(@X,QID,VID,Ret), Buf:=f_empty().

idb3 pIterate(@X,QID,N) :- provQuery(@X,QID,VID,Ret), N:=1.
idb4 pIterate(@X,QID,N) :- pIterate(@X,QID,N1),
       pQList(@X,QID,List), N1<f_size(List), N:=N1+1.

idb5 eRuleQuery(@X,NQID,RID) :- pIterate(@X,QID,N),
       pQList(@X,QID,List), N<=f_size(List),
       RID:=f_item(List,N), NQID:=f_sha1(""+QID+RID).
idb6 ruleQuery(@RLoc,QID,RID,X) :- eRuleQuery(@X,QID,RID),
       prov(@X,VID,RID,RLoc).

idb7 pResultTmp(@X,QID,Ret,VID,Buf) :- rReturn(@X,NQID,RID,Prov),
       pResultTmp(@X,QID,Ret,VID,Buf1), NQID==f_sha1(""+QID+RID),
       Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

idb8 ePReturn(@X,QID) :- pResultTmp(@X,QID,Ret,VID,Buf),
       pQList(@X,QID,List),
       f_size(Buf)==f_size(List), f_size(Buf)!=0.

idb9 pReturn(@Ret,QID,VID,Prov) :- ePReturn(@X,QID),
       pResultTmp(@X,QID,Ret,VID,Buf), Prov:=f_pIDB(Buf,X).

/* Rule Vertex */
rv1 rQList(@X,QID,List) :- ruleQuery(@X,QID,RID,Ret),
      ruleExec(@X,RID,R,List).
rv2 rResultTmp(@X,QID,Ret,RID,Buf) :-
      ruleQuery(@X,QID,RID,Ret), Buf:=f_empty().

rv3 rIterate(@X,QID,N) :- ruleQuery(@X,QID,RID,Ret), N:=1.
rv4 rIterate(@X,QID,N) :- rIterate(@X,QID,N1),
      rQList(@X,QID,List), N1<f_size(List), N:=N1+1.

rv5 eProvQuery(@X,NQID,VID) :- rIterate(@X,QID,N),
      rQList(@X,QID,List),
      VID:=f_item(List,N), NQID:=f_sha1(""+QID+VID).
rv6 provQuery(@X,QID,VID,X) :- eProvQuery(@X,QID,VID).

rv7 rResultTmp(@X,QID,Ret,RID,Buf) :- pReturn(@X,NQID,VID,Prov),
      rResultTmp(@X,QID,Ret,RID,Buf1), NQID==f_sha1(""+QID+VID),
      Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

rv8 eRReturn(@X,QID) :- rResultTmp(@X,QID,Ret,RID,Buf),
      rQList(@X,QID,List), f_size(Buf)==f_size(List).
rv9 rReturn(@Ret,QID,RID,Prov) :- eRReturn(@X,QID),
      rResultTmp(@X,QID,Ret,RID,Buf),
      ruleExec(@X,RID,R,List), Prov:=f_pRULE(Buf,X,R).




/*Querying Rules*/

%% q1 recvCount(@Node,Src,Dst,Data, a_COUNT<*>):-
%% recvPacket(@Node,Src,Dst,Data),
%% recordSD(@Node,Src,Dst).

q1 recvCount(@Node,Src,Dst,Data,a_COUNT<*>):-
   recvPacket(@Node,Src,Dst,Data),
   recordSD(@Node,Src,Dst).


q2 recordSD(@Node,Src,Dst):-
   recvCount(@Node,Src,Dst,Data,Rcount),
   Rcount==0.

q3 initProvQuery(@Node,QID,UID,Dst):-
   recvCount(@Node,Src,Dst,Data,Rcount),
   Rcount==0,
   Time:=f_now(),
   UID:=f_sha1("recvPacket"+Node+Src+Dst+Data),
   QID:=f_sha1(""+UID+Time).

q4 provQuery(@Node,QID,UID,Dst):-
   initProvQuery(@Node,QID,UID,Dst).

q5 recordProv(@Node,QID,UID,Prov):-
   initProvQuery(@Node,QID,UID,Dst),
   pReturn(@Node,QID,UID,Prov).
