/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 1999-2006 Soeren Sonnenburg
 * Written (W) 1999-2006 Gunnar Raetsch
 * Copyright (C) 1999-2006 Fraunhofer Institute FIRST and Max-Planck-Society
 */

#include "features/Labels.h"
#include "lib/common.h"
#include "lib/File.h"
#include "lib/io.h"

CLabels::CLabels()
{
	labels = NULL;
	num_labels = 0;
}

CLabels::CLabels(INT num_lab) : num_labels(num_lab)
{
	labels=new DREAL[num_lab];
	ASSERT(labels);

	for (INT i=0; i<num_lab; i++)
		labels[i]=0;
}

CLabels::CLabels(CHAR* fname)
{
	num_labels=0;
	labels=NULL;

	load(fname);
}

CLabels::~CLabels()
{
	delete[] labels;
	num_labels=0;
	labels=NULL;
}

void CLabels::set_labels(DREAL* labels, INT len)
{
	ASSERT(len>0);
	num_labels = len;

	this->labels = new DREAL[len];
	ASSERT(this->labels);

	for (INT i=0; i<len; i++)
		this->labels[i]=labels[i];
}

DREAL* CLabels::get_labels(INT &len)
{
	len=num_labels;

	if (num_labels>0)
	{
		DREAL* labels=new DREAL[num_labels] ;
		for (INT i=0; i<len; i++)
			labels[i]=get_label(i) ;
		return labels ;
	}
	else 
		return NULL;
}

void CLabels::get_labels(DREAL** labels, INT* len)
{
	ASSERT(labels && len);
	*labels=NULL;
	*len=num_labels;

	if (num_labels>0)
	{
		*labels=new DREAL[num_labels];

		for (INT i=0; i<num_labels; i++)
			(*labels)[i]=get_label(i);
	}
}

INT* CLabels::get_int_labels(INT &len)
{
	len=num_labels;

	if (num_labels>0)
	{
		INT* labels=new INT[num_labels] ;
		for (INT i=0; i<len; i++)
			labels[i]= (INT) get_label(i) ;
		return labels ;
	}
	else 
		return NULL;
}

void CLabels::set_int_labels(INT * mylabels, INT len)
{
	num_labels = len ;
#ifndef HAVE_SWIG
	delete[] labels ;
#endif
	
	labels = new DREAL[num_labels] ;
	for (INT i=0; i<num_labels; i++)
		set_int_label(i, mylabels[i]) ;
}

bool CLabels::load(CHAR* fname)
{
	bool status=false;

	delete[] labels;
	num_labels=0;

	CFile f(fname, 'r', F_DREAL);
	LONG num_lab=0;
	labels=f.load_real_data(NULL, num_lab);
	num_labels=num_lab;

    if (!f.is_ok()) {
#ifdef HAVE_PYTHON
      throw LabelException("loading file \"%s\" failed", fname);
#else
		CIO::message(M_ERROR, "loading file \"%s\" failed", fname);
#endif
    }
	else
	{
		CIO::message(M_INFO, "%ld labels successfully read\n", num_labels);
		status=true;
	}

	return status;
}

bool CLabels::save(CHAR* fname)
{
	return false;
}
