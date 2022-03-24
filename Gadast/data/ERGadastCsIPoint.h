/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERGadastCsIPoint_H
#define ERGadastCsIPoint_H

#include "TVector3.h"

#include "FairMCPoint.h"

/** @class ERGadastCsIPoint
 ** @brief The data class for storing pieces of charged tracks in sensitive volumes in CsI crystall 
 ** @author V.Schetinin <sch_vitaliy@mail.ru>
 ** @version 1.0
**/
class ERGadastCsIPoint : public FairMCPoint 
{

public:
 
  /** @brief Default constructor **/
  ERGadastCsIPoint();

  /** @brief Standard constructor
   ** @param EventID  Index of Event
   ** @param trackID  Index of MCTrack
   ** @param mot0trackID Index of Mother MCTrack
   ** @param posIn    Ccoordinates at entrance of point [cm]
   ** @param posOut   Coordinates at exit of point [cm]
   ** @param momIn    Momentum of track at entrance [GeV]
   ** @param momOut   Momentum of track at exit [GeV]
   ** @param tof      Time since event start [ns]
   ** @param length   Track length since creation [cm]
   ** @param eLoss    Energy deposit [KeV]
   ** @param pdg      PDG of point particle
   ** @param wall     Gadast CsI Wall number
   ** @param block    Gadast CsI Block number
   ** @param cell     Gadast CsI Cell number
  **/
  ERGadastCsIPoint(Int_t eventID, Int_t trackID,
      Int_t mot0trackID,
      Double_t mass,
      TVector3 posIn,
      TVector3 posOut, TVector3 momIn, TVector3 momOut,
      Double_t tof, Double_t length, Double_t eLoss, Int_t pdg,
      Int_t wall, Int_t block, Int_t cell, int parent_gamma_track_id);

  /** @brief Destructor **/
  virtual ~ERGadastCsIPoint();

  /** @brief operator= **/
  ERGadastCsIPoint& operator=(const ERGadastCsIPoint&) { return *this; }

  /** Accessors **/

  Int_t GetEventID()              const { return fEventID; }
  Int_t GetMot0TrackID()          const { return fMot0TrackID; }
  Int_t GetParentGammaTrackID()  const { return fParentGammaTrackId; }
  Double_t GetXIn()               const { return fX; }
  Double_t GetYIn()               const { return fY; }
  Double_t GetZIn()               const { return fZ; }
  Double_t GetXOut()              const { return fX_out; }
  Double_t GetYOut()              const { return fY_out; }
  Double_t GetZOut()              const { return fZ_out; }
  Double_t GetPxOut()             const { return fPx_out; }
  Double_t GetPyOut()             const { return fPy_out; }
  Double_t GetPzOut()             const { return fPz_out; }
  Double_t GetMass()              const { return fMass; }
  Int_t GetWall()                 const { return fWall; }
  Int_t GetBlock()                const { return fBlock; }
  Int_t GetCell()                 const { return fCell; }
  void PositionIn(TVector3& pos)  const { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) const { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) const { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }

  /** @brief Point X coordinate at given z from linear extrapolation **/
  Double_t GetX(Double_t z) const;

  /** @brief Point Y coordinate at given z from linear extrapolation **/
  Double_t GetY(Double_t z) const;

  /** @brief Check for distance between in and out **/
  Bool_t IsUsable() const;

public:

  /** @brief Print point information to screen **/
  virtual void Print(const Option_t* opt = 0) const;

protected:
  
  Int_t fEventID;                                       ///< event index
  Int_t fMot0TrackID;                                   ///< 0th mother track index
  Int_t fParentGammaTrackId = -1;                      ///< id of parrent gamma
  Double_t fMass;                                       ///< mass
  Double32_t fX_out,  fY_out,  fZ_out;                  ///< point out coordinate
  Double32_t fPx_out, fPy_out, fPz_out;                 ///< point out momentum
  Int_t fPDG;                                           ///< particle PDG
  Int_t fWall;                                          ///< CsI Wall number
  Int_t fBlock;                                         ///< CsI Block number
  Int_t fCell;                                          ///< CsI Cell number

  ClassDef(ERGadastCsIPoint,1)
};

#endif
