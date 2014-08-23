#ifndef _MThreadPool
#define _MThreadPool
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors,  which is protected by U.S. and Canadian federal copyright law 
// and by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be 
// copied or duplicated, in whole or in part, without the prior written 
// consent of Autodesk.
// 
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the following 
// disclaimer, must be included in all copies of the Software, in whole 
// or in part, and all derivative works of the Software, unless such copies 
// or derivative works are solely in the form of machine-executable object 
// code generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, 
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO 
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST 
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR 
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS 
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES. 
// ==========================================================================
//+
//
// CLASS:    MThreadPool
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MThreadPool)
//
// The MThreadPool class provides users with a threadpool that can be used for
// fork/join parallelism
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>

#ifdef _WIN32
typedef long MThreadRetVal;
#else
typedef void * MThreadRetVal;
#endif

class MThreadRootTask;
typedef MThreadRetVal (*MThreadFunc)(void *);
typedef void (*MThreadCallbackFunc)(void *, MThreadRootTask *);

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MThreadPool)
/**
*/

/// Methods for creating and managing a thread pool. (OpenMaya)
class OPENMAYA_EXPORT MThreadPool  
{

public:
	///
	static MStatus init();

	///
	static MStatus newParallelRegion(MThreadCallbackFunc func, void* data);

	///
	static MStatus createTask(MThreadFunc func, void* data, MThreadRootTask* root);

	///
	static MStatus executeAndJoin(MThreadRootTask* root);

	///
	static void release();

private:
	MThreadPool() {};
	MThreadPool(const MThreadPool&) {}; // disallow this
	~MThreadPool() {};

	static const char* 	className();
};

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MThreadPool */
