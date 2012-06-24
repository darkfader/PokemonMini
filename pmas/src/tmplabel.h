#ifndef _TMPLABEL_H
#define _TMPLABEL_H

/*
 * TmpLabelItem
 */
struct TmpLabelItem
{
	int goffset;
	int addr;
	TmpLabelItem *left;
	TmpLabelItem *right;

	TmpLabelItem(int addr);
};

/*
 * Variables
 */
extern TmpLabelItem *tmplabel_left;
extern TmpLabelItem *tmplabel_right;
extern int tmplabel_goffset;		// reset to 0 for each pass


/*
 * Prototypes
 */
TmpLabelItem *FindTmpLabel(int depth);
TmpLabelItem *NewTmpLabel(int addr);

#endif	// _TMPLABEL_H
