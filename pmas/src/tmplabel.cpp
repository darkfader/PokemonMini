#include <stdlib.h>
#include "pmas.h"
#include "tmplabel.h"

TmpLabelItem *tmplabel_left = 0;
TmpLabelItem *tmplabel_right = 0;
int tmplabel_goffset;

TmpLabelItem::TmpLabelItem(int addr)
{
	this->goffset = tmplabel_goffset;
	this->addr = addr;
}

TmpLabelItem *FindTmpLabel(int depth)
{
	TmpLabelItem *m;
	if (depth < 0)
	{
		m = tmplabel_right;
		while (m)
		{
			if (m->goffset <= tmplabel_goffset) {
				while (++depth != 0)
				{
					m = m->left;
					if (m == 0) return 0;
				}
				return m;
			}
			m = m->left;
		}
	}
	else if (depth > 0)
	{
		m = tmplabel_left;
		while (m)
		{
			if (m->goffset >= tmplabel_goffset) {
				while (--depth != 0)
				{
					m = m->right;
					if (m == 0) return 0;
				}
				return m;
			}
			m = m->right;
		}
	}
	else
	{
		m = tmplabel_left;
		while (m)
		{
			if (m->goffset == tmplabel_goffset) {
				return m;
			}
			m = m->right;
		}
	}
	return 0;
}

TmpLabelItem *NewTmpLabel(int addr)
{
	TmpLabelItem *m, *tl;
	m = FindTmpLabel(0);
	if (m) {
		eprintf("Internal error in NewTmpLabel\n");
		return 0;
	}
	tl = new TmpLabelItem(addr);
	tl->left = 0;
	tl->right = 0;
	if (!tmplabel_left)
	{
		tmplabel_left = tl;
		tmplabel_right = tl;
	}
	else
	{
		m = tmplabel_right;
		if (m->goffset <= tmplabel_goffset) {
			m->right = tl;
			tl->left = m;
			tmplabel_right = tl;
			return tl;
		}
		while (m)
		{
			if (m->goffset <= tmplabel_goffset) {
				tl->left = m;
				tl->right = m->right;
				m->right = tl;
				return tl;
			}
			m = m->left;
		}
		tmplabel_left->right = tl;
		tl->right = tmplabel_right;
		tmplabel_left = tl;
	}
	return tl;
}
