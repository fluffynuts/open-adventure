#include <stdlib.h>
#include "misc.h"
#include "main.h"
#include "share.h"

/*
 * scoring and wrap-up
 */

void score(long MODE) {
	/* <0 if scoring, >0 if quitting, =0 if died or won */

/*  The present scoring algorithm is as follows:
 *     Objective:          Points:        Present total possible:
 *  Getting well into cave   25                    25
 *  Each treasure < chest    12                    60
 *  Treasure chest itself    14                    14
 *  Each treasure > chest    16                   224
 *  Surviving             (MAX-NUM)*10             30
 *  Not quitting              4                     4
 *  Reaching "game.closng"        25                    25
 *  "Closed": Quit/Killed    10
 *            Klutzed        25
 *            Wrong way      30
 *            Success        45                    45
 *  Came to Witt's End        1                     1
 *  Round out the total       2                     2
 *                                       TOTAL:   430
 *  Points can also be deducted for using hints or too many turns, or for
 *  saving intermediate positions. */

	SCORE=0;
	MXSCOR=0;

/*  First tally up the treasures.  Must be in building and not broken.
 *  Give the poor guy 2 points just for finding each treasure. */

	/* 20010 */ for (I=50; I<=MAXTRS; I++) {
	if(PTEXT[I] == 0) goto L20010;
	K=12;
	if(I == CHEST)K=14;
	if(I > CHEST)K=16;
	if(PROP[I] >= 0)SCORE=SCORE+2;
	if(PLACE[I] == 3 && PROP[I] == 0)SCORE=SCORE+K-2;
	MXSCOR=MXSCOR+K;
L20010: /*etc*/ ;
	} /* end loop */

/*  Now look at how he finished and how far he got.  MAXDIE and game.numdie tell us
 *  how well he survived.  game.dflag will
 *  tell us if he ever got suitably deep into the cave.  game.closng still indicates
 *  whether he reached the endgame.  And if he got as far as "cave closed"
 *  (indicated by "game.closed"), then game.bonus is zero for mundane exits or 133, 134,
 *  135 if he blew it (so to speak). */

	SCORE=SCORE+(MAXDIE-game.numdie)*10;
	MXSCOR=MXSCOR+MAXDIE*10;
	if(MODE == 0)SCORE=SCORE+4;
	MXSCOR=MXSCOR+4;
	if(game.dflag != 0)SCORE=SCORE+25;
	MXSCOR=MXSCOR+25;
	if(game.closng)SCORE=SCORE+25;
	MXSCOR=MXSCOR+25;
	if(!game.closed) goto L20020;
	if(game.bonus == 0)SCORE=SCORE+10;
	if(game.bonus == 135)SCORE=SCORE+25;
	if(game.bonus == 134)SCORE=SCORE+30;
	if(game.bonus == 133)SCORE=SCORE+45;
L20020: MXSCOR=MXSCOR+45;

/*  Did he come to Witt's End as he should? */

	if(PLACE[MAGZIN] == 108)SCORE=SCORE+1;
	MXSCOR=MXSCOR+1;

/*  Round it off. */

	SCORE=SCORE+2;
	MXSCOR=MXSCOR+2;

/*  Deduct for hints/turns/saves.  Hints < 4 are special; see database desc. */

	for (I=1; I<=HNTMAX; I++) {
	if(HINTED[I])SCORE=SCORE-HINTS[I][2];
	} /* end loop */
	if(game.novice)SCORE=SCORE-5;
	if(CLSHNT)SCORE=SCORE-10;
	SCORE=SCORE-game.trnluz-game.saved;

/*  Return to score command if that's where we came from. */

	if(MODE < 0) return;

/*  that should be good enough.  Let's tell him all about it. */

	if(SCORE+game.trnluz+1 >= MXSCOR && game.trnluz != 0)RSPEAK(242);
	if(SCORE+game.saved+1 >= MXSCOR && game.saved != 0)RSPEAK(143);
	SETPRM(1,SCORE,MXSCOR);
	SETPRM(3,game.turns,game.turns);
	RSPEAK(262);
	for (I=1; I<=CLSSES; I++) {
	if(CVAL[I] >= SCORE) goto L20210;
	/*etc*/ ;
	} /* end loop */
	game.spk=265;
	 goto L25000;

L20210: SPEAK(CTEXT[I]);
	game.spk=264;
	if(I >= CLSSES) goto L25000;
	I=CVAL[I]+1-SCORE;
	SETPRM(1,I,I);
	game.spk=263;
L25000: RSPEAK(game.spk);
	exit(0);

}
