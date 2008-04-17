DEFINE loop = 0
DEFINE i = 0
//fprintf("d:\\data2.txt","Start:\n")
FOR loop = 185..210,1
	printf("\n----- %d ------\n\n", loop)
	pausetest
	FOR i = 0..9,1	                 
		fprintf("d:\\data3.txt","%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",loop,i,wh[0],wl[0],wh[1],wl[1],wh[2],wl[2],wh[3],wl[3],wh[4],wl[4],wh[5],wl[5],wh[6],wl[6],wh[7],wl[7])
		wait 2
	ENDFOR
ENDFOR
//fprintf("d:\\data2.txt","End:\n")