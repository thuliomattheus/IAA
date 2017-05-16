
pPerto(X) :- X =< 1.
pLonge(X) :- X > 1.






pMelhor(N, L, O, S)      :- pLonge(N), pLonge(L), pLonge(O), pLonge(S).      %(III) %OK
pMelhor(N, L, O, S)      :- pLonge(N), pLonge(L), pLonge(O), pPerto(S).      %(III) %OK
pMelhor(N, L, O, S)      :- pLonge(N), pLonge(L), pPerto(O), pLonge(S).      %(III) %OK
pNordeste(N, L, O, S)    :- pLonge(N), pLonge(L), pPerto(O), pPerto(S).      %(0)   %OK
pMelhor(N, L, O, S)      :- pLonge(N), pPerto(L), pLonge(O), pLonge(S).      %(III) %OK
pNoroeste(N, L, O, S)    :- pLonge(N), pPerto(L), pLonge(O), pPerto(S).      %(0)   %OK
pVertical(N, L, O, S)    :- pLonge(N), pPerto(L), pPerto(O), pLonge(S).      %(I)   %OK
pNorte(N, L, O, S)       :- pLonge(N), pPerto(L), pPerto(O), pPerto(S).      %(0)   %OK
pMelhor(N, L, O, S)      :- pPerto(N), pLonge(L), pLonge(O), pLonge(S).      %(III) %OK
pHorizontal(N, L, O, S)  :- pPerto(N), pLonge(L), pLonge(O), pPerto(S).      %(II)  %OK
pSudeste(N, L, O, S)     :- pPerto(N), pLonge(L), pPerto(O), pLonge(S).      %(0)   %OK
pLeste(N, L, O, S)       :- pPerto(N), pLonge(L), pPerto(O), pPerto(S).      %(0)   %OK
pSudoeste(N, L, O, S)    :- pPerto(N), pPerto(L), pLonge(O), pLonge(S).      %(0)   %OK
pOeste(N, L, O, S)       :- pPerto(N), pPerto(L), pLonge(O), pPerto(S).      %(0)   %OK
pSul(N, L, O, S)         :- pPerto(N), pPerto(L), pPerto(O), pLonge(S).      %(0)   %OK
pMelhor(N, L, O, S)      :- pPerto(N), pPerto(L), pPerto(O), pPerto(S).      %(III) %OK





pNorte(N, L, O, S)    :- pVertical(N, L, O, S), N >= S.                      %(I)
pSul(N, L, O, S)      :- pVertical(N, L, O, S), S > N.                       %(I)


pLeste(N, L, O, S)    :- pHorizontal(N, L, O, S), L >= O.                    %(II)
pOeste(N, L, O, S)    :- pHorizontal(N, L, O, S), O > L.                     %(II)



pNorte(N, L, O, S)    :- pMelhor(N, L, O, S), N >= L, N >= O, N >= S.        %(III)
pLeste(N, L, O, S)    :- pMelhor(N, L, O, S), L > N,  L >= O, L >= S.        %(III)
pOeste(N, L, O, S)    :- pMelhor(N, L, O, S), O > N,  O > L,  O >= S.        %(III)
pSul(N, L, O, S)      :- pMelhor(N, L, O, S), S > N,  S > L,  S > O.         %(III)



pRobo(VD, VE, N, L, O, S) :- pNorte(N, L, O, S), VD is 0.5, VE is 0.5.
pRobo(VD, VE, N, L, O, S) :- pLeste(N, L, O, S), VD is 0.5, VE is 0.
pRobo(VD, VE, N, L, O, S) :- pOeste(N, L, O, S), VD is 0, VE is 0.5.
pRobo(VD, VE, N, L, O, S) :- pSul(N, L, O, S), VD is -0.5, VE is -0.5.
pRobo(VD, VE, N, L, O, S) :- pNordeste(N, L, O, S), VD is 0.5, VE is 0.2.
pRobo(VD, VE, N, L, O, S) :- pNoroeste(N, L, O, S), VD is 0.2, VE is 0.5.
pRobo(VD, VE, N, L, O, S) :- pSudoeste(N, L, O, S), VD is -0.2, VE is -0.5.
pRobo(VD, VE, N, L, O, S) :- pSudeste(N, L, O, S), VD is -0.5, VE is -0.2.


