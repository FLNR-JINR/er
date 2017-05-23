// -------------------------------------------------------------------------
// -----                      ERFieldPar header file                   -----
// -----                   Created 05/17  by V. Schetinin              -----
// -------------------------------------------------------------------------

#ifndef ERFieldPar_H
#define ERFieldPar_H 1

#include "FairParGenericSet.h"

class FairField;
class FairParamList;

const int kMaxFieldMapType = 5;        
const int kTypeDistorted = 4;        

class ERFieldPar : public FairParGenericSet
{
 public:
  /** Standard constructor  **/
  ERFieldPar(const char* name, const char* title, const char* context);
  /** default constructor  **/
  ERFieldPar();

  /** Destructor **/
  ~ERFieldPar();

  /** Put parameters **/
  virtual void putParams(FairParamList* list);
  /** Get parameters **/
  virtual Bool_t getParams(FairParamList* list);
  /** Set parameters from CbmField  **/
  void SetParameters(FairField* field);

  /** Accessors **/
  Int_t    GetType()      const { return fType; }
  Double_t GetXmin()      const { return fXmin; }
  Double_t GetXmax()      const { return fXmax; }
  Double_t GetYmin()      const { return fYmin; }
  Double_t GetYmax()      const { return fYmax; }
  Double_t GetZmin()      const { return fZmin; }
  Double_t GetZmax()      const { return fZmax; }
  Double_t GetBx()        const { return fBx; }
  Double_t GetBy()        const { return fBy; }
  Double_t GetBz()        const { return fBz; }

  void MapName(TString& name) { name = fMapName; }

  Double_t GetPositionX() const { return fPosX; }
  Double_t GetPositionY() const { return fPosY; }
  Double_t GetPositionZ() const { return fPosZ; }
  Double_t GetScale()     const { return fScale; }

  void     GetDistortionFilename(TString& filename) { filename = fDistortionFilename; }
  void     GetParentName(TString& parentname) { parentname = fParentName; }
  Int_t    GetTypeOfParent()      const { return fTypeOfParent; }

 private:

  /** Field type
   ** 0 = constant field
   ** 1 = field map
   ** 2 = field map sym2 (symmetries in x and y)
   ** 3 = field map sym3 (symmetries in x, y and z)
   ** kTypeDistorted = distorted field map (its parent field can be field map or constant field)
   **/

  Int_t fType;
  /** Field limits in case of constant field **/

  Double_t fXmin, fXmax;
  Double_t fYmin, fYmax;
  Double_t fZmin, fZmax;

  /** Field values in case of constant field [kG] **/
  Double_t fBx, fBy, fBz;

  /** Field map name in case of field map **/
  TString fMapName;

  /** Field centre position for field map **/
  Double_t fPosX, fPosY, fPosZ;

  /** Scaling factor for field map **/
  Double_t fScale;

  /** Field map distortion filename (in case of distorted field map) **/
  TString fDistortionFilename;

  /** Field name of parent field (in case of distorted field map) **/
  TString fParentName;

  /** Field type of parent field (in case of distorted field map) **/
  Int_t fTypeOfParent;

  ERFieldPar(const ERFieldPar&);

  ERFieldPar& operator=(const ERFieldPar&);

  ClassDef(ERFieldPar,1);
};

#endif
