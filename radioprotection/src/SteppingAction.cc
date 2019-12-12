//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// Authors: Susanna Guatelli, susanna@uow.edu.au,
// Authors: Jeremy Davis, jad028@uowmail.edu.au
//

#include "G4ios.hh"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "SteppingAction.hh"
#include "AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(AnalysisManager* pAnalysis)
{ 
analysis = pAnalysis;
fSecondary = 0; 
}

SteppingAction::~SteppingAction()
{ 
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
	// get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

   G4String CurrentParticle = step -> GetTrack() -> GetParticleDefinition() -> GetParticleName();
   
      
   G4ThreeVector pos = step  -> GetPostStepPoint() -> GetPosition();
        
   G4int parent = step ->GetTrack() ->GetParentID();
   
   if(CurrentParticle == "proton" && pos.getX()/mm >= 0. && parent == 0)
   {
     G4double Ekin = step -> GetPostStepPoint() -> GetKineticEnergy();
     analysis -> FillProton(Ekin/MeV, pos.getX()/mm);
   }
   else if(CurrentParticle == "gamma")
   {
   	
     if(volume->GetName() == "TumorLV" && step -> GetPostStepPoint() -> GetStepStatus() == fGeomBoundary)
       {
	 G4double Ekin = step -> GetPostStepPoint() -> GetKineticEnergy();
	 G4ThreeVector direction = step -> GetPostStepPoint() -> GetMomentumDirection();
	 analysis->FillExitTumor(Ekin/MeV, pos.getX()/mm, pos.getY()/mm, pos.getZ()/mm, direction.getX(), direction.getY(), direction.getZ(), 0., 0.);
       }
     if(volume->GetName() == "WaterPhantomBoneLV" && step -> GetPostStepPoint() -> GetStepStatus() == fGeomBoundary)
       {
	 G4double Ekin = step -> GetPostStepPoint() -> GetKineticEnergy();
	 G4ThreeVector direction = step -> GetPostStepPoint() -> GetMomentumDirection();
	 analysis->FillExitAbsorber(Ekin/MeV, pos.getX()/mm, pos.getY()/mm, pos.getZ()/mm, direction.getX(), direction.getY(), direction.getZ(), 0., 0.);
       }
   }

}
