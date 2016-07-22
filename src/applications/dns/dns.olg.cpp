materialize(url,infinity,infinity,keys(1,2:str,3,4:int32)).
materialize(address_record,infinity,infinity,keys(1,2:str,3)).
materialize(name_server,infinity,infinity,keys(1,2:str,3:str)).
materialize(result,infinity,infinity,keys(1,2:str,3,4:int32)).

r1 request(@ADDR,URL,HOST,REQUESTID) :- url(@X,URL,HOST,REQUESTID),name_server(@X,DOMAIN,SERVER),address_record(@X,SERVER,ADDR),f_indexof(URL,DOMAIN)!=-1,URL!=DOMAIN.

r2 request(@ADDR,URL,HOST,REQUESTID) :- request(@X,URL,HOST,REQUESTID),name_server(@X,DOMAIN,SERVER),
          address_record(@X,SERVER,ADDR),f_indexof(URL,DOMAIN)!=-1,
       URL!=DOMAIN.

r3 result(@ADDR,URL,HOST,REQUESTID) :- request(@X,URL,HOST,REQUESTID),name_server(@X,DOMAIN,SERVER),
    address_record(@X,SERVER,ADDR),f_indexof(URL,DOMAIN)!=-1,
 URL==DOMAIN.
