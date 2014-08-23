#ifndef _hlslShader_h_
#define _hlslShader_h_

// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc. and/or its licensors.  All 
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors, which is protected by U.S. and Canadian federal copyright 
// law and by international treaties.
//
// The Data is provided for use exclusively by You. You have the right 
// to use, modify, and incorporate this Data into other products for 
// purposes authorized by the Autodesk software license agreement, 
// without fee.
//
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the 
// following disclaimer, must be included in all copies of the 
// Software, in whole or in part, and all derivative works of 
// the Software, unless such copies or derivative works are solely 
// in the form of machine-executable object code generated by a 
// source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
// PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE, OR 
// TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS LICENSORS 
// BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL, 
// DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK 
// AND/OR ITS LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY 
// OR PROBABILITY OF SUCH DAMAGES.
//
// ==========================================================================
//+

#include <maya/MPxHardwareShader.h>
#include <maya/MTypeId.h> 
#include <maya/MStringArray.h> 
#include <maya/MVaryingParameter.h>
#include <maya/MUniformParameter.h>
#include <maya/MUniformParameterList.h>
#include <maya/MRenderProfile.h>

#include <maya/MHwrCallback.h>

#include <d3d9.h>
#include <d3dx9.h>

/*
//
// A varying parameter
//
class hlslVaryingParameter 
{
public:
	MVaryingParameter	fMayaParameter;
};


//
// A pass
//
class hlslPass
{
public:
	inline				hlslPass( 0) : fNumParameters( 0), fParameters( NULL) {}
	inline				~hlslPass() { if( fParameters) delete fParameters; }
	inline int			numParameters() const { return fNumParameters; }
	inline void			numParameters( int numParms) { if( fParameters) delete fParameters; fNumParameters = numParms; fParameters = numParms ? new hlslVaryingParameter[ numParms] : NULL; }
	inline hlslVaryingParameter& parameter( int p) const { return fParameters[ p]; }

protected:
	hlslVaryingParameter* fParameters;
	int					fNumParameters;
};
*/

class hlslShader;

// Callback structure to handle device changes
//
class hlslDeviceManager : public MHwrCallback 
{
public:
	typedef enum
	{
		kInvalid,
		kReset,
		kValid
	} hlslDeviceState;
	inline hlslDeviceManager( hlslShader& shader) : fShader( shader), fState( kValid) { addCallback( this); }
	~hlslDeviceManager() { removeCallback( this); }
	virtual void deviceNew();	
	virtual void deviceLost();
	virtual void deviceReset();
	virtual void deviceDeleted();
	inline hlslDeviceState  deviceState() { return fState; }
	void resetShader();

private:
	// Hide this bad boy to let the compiler know it shouldn't be able to generate an assignment operator
	void operator=( const hlslDeviceManager&) {};

protected:
	hlslDeviceState fState;
	hlslShader&	fShader;
};

// Our shader node itself!
//
class hlslShader : public MPxHardwareShader
{
public:

	friend class hlslDeviceManager;

	// PxNode housekeeping: create, copy setup
						hlslShader();
	virtual void        postConstructor();
	virtual				~hlslShader(); 
	static  void*		creator();
	static  MStatus		initialize();
	static  void        initializeNodeAttrs();

	// Internal attribute housekeeping
	virtual void		copyInternalData( MPxNode* pSrc );
	virtual bool		getInternalValueInContext( const MPlug&,
											  MDataHandle&,
											  MDGContext&);
    virtual bool		setInternalValueInContext( const MPlug&,
											  const MDataHandle&,
											  MDGContext&);

	// Override geometry rendering
	virtual MStatus		render( MGeometryList& iterator);

	// Indicate pass transparency options 
	virtual unsigned int	transparencyOptions();

	// Query the renderers supported by this shader
	//
	virtual const MRenderProfile& profile();

	// Override this method to draw a image for swatch rendering.
	///
	virtual MStatus renderSwatchImage( MImage & image );
public:

	// Release all our device handles
	void				release();
	
	// The typeid is a unique 32bit indentifier that describes this node.
	// It is used to save and retrieve nodes of this type from the binary
	// file format.  If it is not unique, it will cause file IO problems.
	//
	static  MTypeId sId;

	// Our rendering profile
	//
	static MRenderProfile sProfile;

	// There needs to be a MObject handle declared for each attribute that
	// the node will have.  These handles are needed for getting and setting
	// the values later.
	//
	// Input shader attribute
	//
	static  MObject	sShader;
	static  MObject	sTechnique;
	static  MObject	sTechniques;
	static  MObject	sDescription;
	static  MObject	sDiagnostics;
	
protected:

	// Change the shader or technique we're using
	MStatus			setShader( const MString& shader);
	MStatus			setTechnique( const MString& technique);
	bool			passHasTranparency( D3DXHANDLE passHandle ); 

	// Resource management
	hlslDeviceManager	fDeviceManager;
	void			releaseVertexDeclaration();

	// Values of internal attributes
	MString			fShader;
	MString			fTechnique;
	MStringArray	fTechniques;

	// The current daignostics/description strings
	MString			fDescription;
	MString			fDiagnostics;

	// D3D resources
	LPD3DXEFFECT	fD3DEffect;
	D3DXHANDLE		fD3DTechnique;
	D3DXEFFECT_DESC fD3DEffectDesc;
	D3DXTECHNIQUE_DESC fD3DTechniqueDesc;


	// The current set of parameters
	MVaryingParameter fVertexStructure;
	IDirect3DVertexDeclaration9** fD3DVertexDeclaration;
	MUniformParameterList fUniformParameters;
	bool				  fTechniqueHasBlending;

	// Utilities for converting D3D -> Maya
	void			setupUniform( D3DXHANDLE d3dParameter, const MString& prefix);
	bool			GetAnnotation( D3DXHANDLE parameter, const char* name, LPCSTR& value);
	bool			GetAnnotation( D3DXHANDLE parameter, const char* name, BOOL& value);
	MUniformParameter::DataSemantic ConvertSemantic( D3DXHANDLE parameter, D3DXPARAMETER_DESC& description);
	MUniformParameter::DataType		ConvertType( D3DXHANDLE parameter, D3DXPARAMETER_DESC& description);
	MUniformParameter::DataSemantic ConvertSpace( D3DXHANDLE parameter, D3DXPARAMETER_DESC& description, MUniformParameter::DataSemantic defaultSpace);

	// Ummm ... don't ask
	inline hlslShader& me() { return *this; }

	// Housekeeping
	int				fErrorCount;
	void			reportInternalError( const char* function, size_t errcode );
};

#endif /* _hlslShader_h_ */
