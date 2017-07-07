// -------------------------------------------------------------------------
// -----                      ERFieldMap header file                   -----
// -----                   Created 05/17  by V.Schetinin               -----
// -------------------------------------------------------------------------
/** ERFieldMap.h
 **
 ** Magnetic field map on a 3-D grid.
 ** Field values are hold and returned in kG.
 **/
#ifndef ERFieldMap_H
#define ERFieldMap_H 1
#include "FairField.h"

class TArrayF;

class ERFieldPar;

class ERFieldMap : public FairField {
public:
  /** Default constructor **/
  ERFieldMap();
  /** Standard constructor
   ** @param name       Name of field map
   ** @param fileType   R = ROOT file, A = ASCII
   **/
  ERFieldMap(const char* mapName, const char* fileType = "R");
  /** Constructor from ERFieldPar **/
  ERFieldMap(ERFieldPar* fieldPar);
  /** Destructor **/
  virtual ~ERFieldMap();
  /** Initialisation (read map from file) **/
  virtual void Init();
  /** Get the field components at a certain point 
   ** @param x,y,z     Point coordinates (global) [cm]
   ** @value Bx,By,Bz  Field components [kG]
   **/
  virtual Double_t GetBx(Double_t x, Double_t y, Double_t z);
  virtual Double_t GetBy(Double_t x, Double_t y, Double_t z);
  virtual Double_t GetBz(Double_t x, Double_t y, Double_t z);
  /** Determine whether a point is inside the field map
   ** @param x,y,z              Point coordinates (global) [cm]
   ** @param ix,iy,iz (return)  Grid cell
   ** @param dx,dy,dz (return)  Distance from grid point [cm] if inside
   ** @value kTRUE if inside map, else kFALSE
   **/
  virtual Bool_t IsInside(Double_t x, Double_t y, Double_t z,
                          Int_t& ix, Int_t& iy, Int_t& iz,
                          Double_t& dx, Double_t& dy, Double_t& dz);
 
  /** Write the field map to an ASCII file **/
  void WriteAsciiFile(const char* fileName);
                                                                
  /** Write field map data to a ROOT file **/
  void WriteRootFile(const char* fileName, const char* mapName);
  /** Set the position of the field centre **/
  virtual void SetPosition(Double_t x, Double_t y, Double_t z);
  /** Set a global field scaling factor **/
  virtual void SetScale(Double_t factor) { fScale = factor; }
  /** Accessors to field parameters in local coordinate system **/
  Double_t GetXmin()  const { return fXmin; } 
  Double_t GetYmin()  const { return fYmin; }
  Double_t GetZmin()  const { return fZmin; }
  Double_t GetXmax()  const { return fXmax; }  
  Double_t GetYmax()  const { return fYmax; }
  Double_t GetZmax()  const { return fZmax; }
  Double_t GetXstep() const { return fXstep; }  
  Double_t GetYstep() const { return fYstep; }
  Double_t GetZstep() const { return fZstep; }
  Int_t    GetNx()    const { return fNx; }
  Int_t    GetNy()    const { return fNy; }
  Int_t    GetNz()    const { return fNz; }
  /** Accessor to field centre position in global system **/
  Double_t GetPositionX() const { return fPosX; }
  Double_t GetPositionY() const { return fPosY; }
  Double_t GetPositionZ() const { return fPosZ; }
  /** Accessor to global scaling factor  **/
  Double_t GetScale() const { return fScale; }
  /** Accessors to the field value arrays **/
  TArrayF* GetBx() const { return fBx; }
  TArrayF* GetBy() const { return fBy; }
  TArrayF* GetBz() const { return fBz; }
  /** Accessor to field map file **/
  const char* GetFileName() { return fFileName.Data(); }
  /** Screen output **/
  virtual void Print(Option_t* = "") const;
        
        
 protected:
  /** Reset the field parameters and data **/
  void Reset();
  /** Read the field map from an ASCII file **/
  void ReadAsciiFile(const char* fileName);
  /** Read field map from a ROOT file **/        
  void ReadRootFile(const char* fileName, const char* mapName);
  /** Set field parameters and data **/
  /*void SetField(const ERFieldMapData* data);*/
  /** Get field values by interpolation of the grid.
   ** @param dx,dy,dz  Relative distance from grid point [cell units]
   **/
  Double_t Interpolate(Double_t dx, Double_t dy, Double_t dz); 
  /** Map file name **/
  TString fFileName;
  /** Global scaling factor (w.r.t. map on file) **/
  Double_t fScale;             
  /** Field centre position in global coordinates  **/
  Double_t fPosX, fPosY, fPosZ; 
  /** Field limits in local coordinate system **/
  Double_t fXmin, fXmax, fXstep;
  Double_t fYmin, fYmax, fYstep;
  Double_t fZmin, fZmax, fZstep;
  /** Number of grid points  **/
  Int_t fNx, fNy, fNz;
  /** Arrays with the field values  **/
  TArrayF* fBx;
  TArrayF* fBy;
  TArrayF* fBz;
  /** Variables for temporary storage 
   ** Used in the very frequently called method GetFieldValue  **/
  Double_t fHa[2][2][2];            //! Field at corners of a grid cell
  Double_t fHb[2][2];               //! Interpolated field (2-dim)
  Double_t fHc[2];                  //! Interpolated field (1-dim)
  Double_t fBxOrigin;               //! x-component of the field at the origin
  Double_t fByOrigin;               //! y-component of the field at the origin
  Double_t fBzOrigin;               //! z-component of the field at the origin
 private:
  ERFieldMap(const ERFieldMap&);
  ERFieldMap& operator=(const ERFieldMap&);
  ClassDef(ERFieldMap,2) 
};
#endif
