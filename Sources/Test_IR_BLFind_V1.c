#include "includes.h"

#define nIR 12

#define D 20

#define out(x, y, z) {am = x;an = y;return z;}

#define swap(x, y) {(x) = (x) ^ (y);(y) = (x) ^ (y);(x) = (x) ^ (y);}

#define addArea(x, y, z) {areas[p] =(x);areas[p+1] = (y);areas[p+2] = (z);p += 3;}

int ir_area[nIR][4] = {
                          {-1252, -1192, -1052, -992},
                          {-1048, -988, -848, -788},
                          {-844, -784, -644, -584},
                          {-640, -580, -440, -380},
                          {-436, -376, -236, -176},
                          {-232, -172, -32, 28},
                          {-28, 32, 172, 232},
                          {176, 236, 376, 436},
                          {380, 440, 580, 640},
                          {584, 644, 784, 844},
                          {788, 848, 988, 1048},
                          {992, 1052, 1192, 1252}
                      };

int areas[1024],p = 0, am, an;
int va[4],vn;

void blPos(int ir, int v, int hi, int lo) {
    if (v > hi) {
        va[0] = -1250;
        va[1] = ir_area[ir][0];
        va[2] = ir_area[ir][3];
        va[3] = 1250;
        vn = 2;
    } else if (v < lo) {
        va[0] = ir_area[ir][1];
        va[1] = ir_area[ir][2];
        vn = 1;
    } else {
        va[0] = (hi - v) * (ir_area[ir][1] - ir_area[ir][0]) / (hi - lo) + ir_area[ir][0] - D;
        va[1] = va[0] +  2 * D;
        va[2] =  - (hi - v) * (ir_area[ir][3] - ir_area[ir][2]) / (hi - lo) + ir_area[ir][3] - D;
        va[3] = va[2] +  2 * D;
        vn = 2;
    }
}

// 求范围
// (a, b) 和 (c, d) 取交集
int getArea(int a, int b, int c, int d) {
    //printf("(%d %d)^(%d %d) -> ", a, b, c, d);

    if (b < c) {
        out(0, 0, 0);
        //out {i,j}, w = 0
    } else {
        if (b < d) {
            if (a < c) {
                out(c, b, 1);
                // out (a,d), w = 1
            } else {
                out(a, b, 1);
                // out (a,b), w = 1
            }
        } else {
            if (a < c) {
                out(c, d, 1);
                // out (c,d), w = 1
            } else {
                if (a > d) {
                    out(0, 0, 0);
                    // out {i,j}, w = 0
                } else {
                    out(a, d, 1);
                    // out (a,d)
                }
            }
        }
    }
}
/*
void ShowArea() {
	int pa = 0;
	while(areas[pa] != -1) {
		printf("(%d,%d)*%d, ", areas[pa], areas[pa+1], areas[pa+2]);
		pa += 3;
	}
	printf("\n");
}*/

void processArea(int a, int b) {
    int k = 0, tp = p;

    for (k = 0;k < tp;k+=3) {

        if (getArea(a , b, areas[k], areas[k+1]) == 1) {
            addArea(am, an, areas[k+2] + 1);
        }
    }

    areas[p] = a;
    areas[1+p] = b;
    areas[2+p] = 0;
    p += 3;
    //ShowArea();
}

void findArea(void) {
    int i =0;
    int wMax[10], wMaxk = 0, max = -1;
    for (i = 0;i < p;i+=3) {
        if (areas[i + 2] > max ) {
            max = areas[i + 2];
            wMax[0] = i / 3;
            wMaxk = 0;
        } else if (areas[i + 2] == max ) {
            wMax[++wMaxk] = i / 3;
        }
    }

    for (i = 0;i <= wMaxk;i++) {
        //printf("(%d, %d),", areas[wMax[i] * 3], areas[ (wMax[i] * 3 + 1) ]);
        //output result
        printl(1, "%8d", (areas[wMax[i] * 3] + areas[ (wMax[i] * 3 + 1) ]) / 2);
    }
    printl(0, "%8d", (areas[wMax[0]] + areas[ (wMax[0] + 1) ]) / 2);
    //printf("\n");
}

/*
void Test_All() {

	memset(areas, -1, sizeof(areas));
	p = 0;
	blPos(6, 650, 800, 200);
	processArea(va[0], va[1]);
	printf("<<%d %d>>", va[0], va[1]);
	if(vn == 2) {
		processArea(va[2], va[3]);
		printf("<<%d %d>>", va[2], va[3]);
	}

	blPos(7, 199, 800, 200);
	processArea(va[0], va[1]);
	printf("<<%d %d>>", va[0], va[1]);
	if(vn == 2) {
		processArea(va[2], va[3]);
		printf("<<%d %d>>", va[2], va[3]);
	}


	blPos(8, 801, 800, 200);
	processArea(va[0], va[1]);
	printf("<<%d %d>>", va[0], va[1]);
	if(vn == 2) {
		processArea(va[2], va[3]);
		//printf("<<%d %d>>", va[2], va[3]);
	}
	//ShowArea();

	findArea();

}*/
