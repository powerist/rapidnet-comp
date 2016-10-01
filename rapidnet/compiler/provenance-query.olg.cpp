materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(pQList,infinity,infinity,keys(1,2:cid)).
materialize(rQList,infinity,infinity,keys(1,2:cid)).
materialize(pResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).


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
idb6 ruleQuery(@RLoc,NQID,RID,X) :- eRuleQuery(@X,NQID,RID),
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
rv1 rQList(@X,NQID,List) :- ruleQuery(@X,NQID,RID,Ret),
      ruleExec(@X,RID,R,List).
rv2 rResultTmp(@X,NQID,Ret,RID,Buf) :-
      ruleQuery(@X,NQID,RID,Ret), Buf:=f_empty().

rv3 rIterate(@X,NQID,N) :- ruleQuery(@X,NQID,RID,Ret), N:=1.
rv4 rIterate(@X,NQID,N) :- rIterate(@X,NQID,N1),
      rQList(@X,NQID,List), N1<f_size(List), N:=N1+1.

rv5 eProvQuery(@X,NNQID,VID) :- rIterate(@X,NQID,N),
      rQList(@X,NQID,List),
      VID:=f_item(List,N), NNQID:=f_sha1(""+NQID+VID).
rv6 provQuery(@X,NNQID,VID,X) :- eProvQuery(@X,NNQID,VID).

rv7 rResultTmp(@X,NQID,Ret,RID,Buf) :- pReturn(@X,NNQID,VID,Prov),
      rResultTmp(@X,NQID,Ret,RID,Buf1), NNQID==f_sha1(""+NQID+VID),
      Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

rv8 eRReturn(@X,NQID) :- rResultTmp(@X,NQID,Ret,RID,Buf),
      rQList(@X,NQID,List), f_size(Buf)==f_size(List).
rv9 rReturn(@Ret,NQID,RID,Prov) :- eRReturn(@X,NQID),
      rResultTmp(@X,NQID,Ret,RID,Buf),
      ruleExec(@X,RID,R,List), Prov:=f_pRULE(Buf,X,R).
