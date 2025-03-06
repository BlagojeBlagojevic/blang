## CONCURENCY IN BLANG: 
This is a naive implementation of concurency in lang.

Coruthines suport only save ip context. 

IMPLEMENTED:
1. COURUTHINE -> Saves ip context (Implemententhed like a loop ):
```forth
  word COURUTINE 7 IP +  & _courutines0 _courutinesCounter   + ??   while endword
``` 
2. ENDCOURUTINE -> Just a endloop wraper:
```forth
  word ENDCOURUTINE endloop endword 
```
3.  BREAKCOURUTINE -> Just a breakloop wraper:
```forth
  word BREAKCOURUTINE breakloop endword 
```
4. INIT_COURUTINE -> Allocate all stuf we need for a this(Depends on memset to sets up all to be NONINITED -1):
```forth
word MAX_COURUTINE       1024 endword
word NONINITED_COURUTINE -1 endword
word INACTIVE_COURUTINE   1 endword



word INIT_COURUTINE
	arr _courutines MAX_COURUTINE drop
	0 ? _courutinesCounter drop
	(& _courutines0 -1 MAX_COURUTINE) memset	
endword
```
5. MAIN_COURUTINE -> Used for main counter switching(Like a while 1 courutine) :
```forth
  word MAIN_COURUTINE
	COURUTINE
		0 ? _courutinesCounter drop 
		& _courutines0 @ dup
		0 = if
			while 
				_courutinesCounter 1 + ? _courutinesCounter
				& _courutines0 + @ dup dup dup
				-1 = if
					0 10  "NothingInCourutines" 10 printstring
					0 exit
				end
				0 = ! if breakloop end
			endloop
		end
		JMP
	ENDCOURUTINE 
endword
```
6. YIELD_COURUTINE -> Return from courutine:
```forth
  word YIELD_COURUTINE
	 & _courutines0 _courutinesCounter 1 + + @ dup dup  
	 _courutinesCounter 1 + ? _courutinesCounter drop 
	1 >  if
		0 = if  
			while
				_courutinesCounter 1 + ? _courutinesCounter drop
				& _courutines0 _courutinesCounter  + @ dup dup
				-1 = if
					0 ? _courutinesCounter drop
				end
				1 < if dup  JMP end
			endloop
		end
	 	BREAKCOURUTINE
	 end
	 JMP
endword
```
7.   FREE_COURUTINE -> Set up a flag that couruthin is disabeled:
```forth
  word FREE_COURUTINE
	0 & _courutines0 _courutinesCounter + ?? drop 
endword
```
8. TBD: Spawning new courutine programaticly, Enabling disabeled courutine(This is trival just setup flag kinda like free)

### EXAMPLE:
```forth
  0 
word memcpy
	-1 +
	? memcpySize
	drop
	? memcpySrc
	drop
	? memcpyDest
	drop
	SP ? memcpySP
	drop
	while
	
		memcpySrc @ memcpyDest ??
		memcpySrc 1 + ? memcpySrc 
		memcpyDest 1 + ? memcpyDest
		memcpySize 0 = if
			breakloop
		end
		memcpySize -1 + ? memcpySize
		
	endloop
	memcpySP SET
endword

word memset 
	? memsetSize
	drop
	? memsetSrc
	drop
	? memsetDest
	drop
	SP ? memsetSP
	drop
	while
		
		memsetSrc memsetDest ??		
		memsetDest 1 + ? memsetDest
		memsetSize 0 = if
			breakloop
		end
		memsetSize -1 + ? memsetSize
	endloop
	memsetSP SET
endword

word DEBUG 
	printstack halt
endword


word MAX_COURUTINE       1024 endword
word NONINITED_COURUTINE -1 endword
word INACTIVE_COURUTINE   1 endword



word INIT_COURUTINE
	arr _courutines MAX_COURUTINE drop
	0 ? _courutinesCounter drop
	(& _courutines0 -1 MAX_COURUTINE) memset	
endword

word COURUTINE 7 IP +  & _courutines0 _courutinesCounter   + ??   while endword

word ENDCOURUTINE endloop endword 



word BREAKCOURUTINE breakloop endword

word MAIN_COURUTINE
	COURUTINE
		0 ? _courutinesCounter drop 
		& _courutines0 @ dup
		0 = if
			while 
				_courutinesCounter 1 + ? _courutinesCounter
				& _courutines0 + @ dup dup dup
				-1 = if
					0 10  "NothingInCourutines" 10 printstring
					0 exit
				end
				0 = ! if breakloop end
			endloop
		end
		JMP
	ENDCOURUTINE 
endword

word YIELD_COURUTINE
	 & _courutines0 _courutinesCounter 1 + + @ dup dup  
	 _courutinesCounter 1 + ? _courutinesCounter drop 
	1 >  if
		0 = if  
			while
				_courutinesCounter 1 + ? _courutinesCounter drop
				& _courutines0 _courutinesCounter  + @ dup dup
				-1 = if
					0 ? _courutinesCounter drop
				end
				1 < if dup  JMP end
			endloop
		end
	 	BREAKCOURUTINE
	 end
	 JMP
endword

word FREE_COURUTINE
	0 & _courutines0 _courutinesCounter + ?? drop 
endword



0 ? i1 drop
word counter1
	COURUTINE
		i1 print  drop
		0 32 32 "Corutine1"  printstring halt 0 SET
		i1 1 + ? i1 10 = if FREE_COURUTINE end
 		YIELD_COURUTINE
	ENDCOURUTINE
endword

0 ? i2 drop
word counter2
	COURUTINE
		i2 print drop
		0 32 32 "Corutine2" printstring halt 0 SET
		i2 1 + ? i2 15 = if FREE_COURUTINE end
		YIELD_COURUTINE
	ENDCOURUTINE
endword

0 ? i3 drop
word counter3
	COURUTINE
		i3 print drop
		0 32 32 "Corutine3" printstring halt 0 SET
		i3 1 + ? i3 7 = if FREE_COURUTINE end
		YIELD_COURUTINE
	ENDCOURUTINE
endword





INIT_COURUTINE


(counter1) 

(counter2)

(counter3)

COURUTINE 0 10 "LoopCoru" 10 printstring 0 SET YIELD_COURUTINE ENDCOURUTINE

MAIN_COURUTINE ()

printstack

endscript .
```       
  
