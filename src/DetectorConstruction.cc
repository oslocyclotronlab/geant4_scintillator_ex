///////////////////////////////////////////////////////////////////
//
// Apr/2015  E. Nacher -> DetectorConstruction.cc
//
///////////////////////////////////////////////////////////////////

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

using namespace CLHEP;

DetectorConstruction::DetectorConstruction()
{ }

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	
	//----------------------------------------------------
	// Material definitions
	//----------------------------------------------------
	
	G4String name, symbol;             //a=mass of a mole;
	G4double a, z, density;            //z=mean number of protons;  
	
	G4int ncomponents, natoms;         
	
	G4double pressure    = 3.e-18*pascal;
	G4double temperature = 2.73*kelvin;
	density     = 1.e-25*g/cm3;
	
	G4Material* Vacuum = new G4Material(name="Galactic", z=1., a=1.01*g/mole, 
										density,kStateGas,temperature,pressure);
	
	
	//
	// define simple Elements
	//
	
	// O
	a = 15.999*g/mole;
	G4Element* O  = new G4Element(name="Oxygen" ,symbol="O" , z= 8., a);
		
	// Mg
	a = 24.305*g/mole;
	G4Element* Mg  = new G4Element(name="Magnesium"  ,symbol="Mg" , z= 12., a);
	
	// Si
	a = 28.0855*g/mole;
	G4Element* Si  = new G4Element(name="Silicon"  ,symbol="Si" , z= 14., a);
	
	// Cl
	a = 35.453*g/mole;
	G4Element* Cl  = new G4Element(name="Chlorine"  ,symbol="Cl" , z= 17., a);
	
	// K
	a = 39.0983*g/mole;
	G4Element* K  = new G4Element(name="Potassium"  ,symbol="K" , z= 19., a);
	
	// Br
	a = 79.904*g/mole;
	G4Element* Br  = new G4Element(name="Bromine"  ,symbol="Br" , z= 35., a);
	
	// Sb
	a = 121.76*g/mole;
	G4Element* Sb = new G4Element(name="Antimony",symbol="Sb" , z= 51., a);
	
	// Cs
	a = 132.905*g/mole;
	G4Element* Cs = new G4Element(name="Cesium",symbol="Cs" , z= 55., a);
	
	// La
	a = 138.905*g/mole;
	G4Element* La = new G4Element(name="Lanthanum",symbol="La" , z= 57., a);
	
	
	//
	// define simple materials
	//
	
	// Al reflector
	density = 2.700*g/cm3;
	a = 26.98*g/mole;
	G4Material* AluR = new G4Material(name="AluRef", z=13., a, density);
	
	// MgO reflector
	density = 2.0*g/cm3;
	G4Material* MgO = new G4Material(name="MgO", density, ncomponents=2);
	MgO->AddElement(Mg, natoms=1);
	MgO->AddElement(O, natoms=1);
		
	// LaCl3
	density = 3.85*g/cm3;
	G4Material* LaCl3 = new G4Material(name="LaCl3", density, ncomponents=2);
	LaCl3->AddElement(Cl, natoms=3);
	LaCl3->AddElement(La, natoms=1);
	
	// LaBr3
	density = 5.08*g/cm3;
	G4Material* LaBr3 = new G4Material(name="LaBr3", density, ncomponents=2);
	LaBr3->AddElement(Br, natoms=3);
	LaBr3->AddElement(La, natoms=1);
	
	// Quartz (fused silica)
	density = 2.20*g/cm3;
	G4Material* Quartz = new G4Material(name="Quartz", density, ncomponents=2);
	Quartz->AddElement(Si, natoms=1);
	Quartz->AddElement(O, natoms=2);
	
	// Photocathode Material 
	// (Bialkali K2CsSb,  Density=?, Thickness=25.*nm?)
	density = 2.00*g/cm3;
	G4Material* K2CsSb = new G4Material(name="K2CsSb", density, ncomponents=3);
	K2CsSb->AddElement(K, natoms=2);
	K2CsSb->AddElement(Cs, natoms=1);
	K2CsSb->AddElement(Sb, natoms=1);
	
	//------------------------------------------------------
	// Optical properties
	//------------------------------------------------------
	
	const G4int nEntries = 2;
	
	G4double PhotonEnergy[nEntries] = {1.0*eV,7.0*eV};
	
	// MgO reflector
	
	G4double MgORefractionIndex[nEntries] = {1.0,1.0};
	
	G4double MgOAbsorptionLength[nEntries] = {1.0E-9*m,1.0E-9*m};
	
	G4MaterialPropertiesTable* MgOMPT = new G4MaterialPropertiesTable();
	
	MgOMPT->AddProperty("RINDEX",PhotonEnergy,MgORefractionIndex,
						nEntries);
	MgOMPT->AddProperty("ABSLENGTH",PhotonEnergy,MgOAbsorptionLength,
						nEntries);
	
	MgO->SetMaterialPropertiesTable(MgOMPT);
	
	// LaBr3
	
	G4double LaBr3RefractionIndex[nEntries] = {1.9,1.9};
	
	G4double LaBr3AbsorptionLength[nEntries] = {50.*cm,50.*cm};
	
	G4MaterialPropertiesTable* LaBr3MPT = new G4MaterialPropertiesTable();
	
	LaBr3MPT->AddProperty("RINDEX",PhotonEnergy,LaBr3RefractionIndex,
						  nEntries);
	LaBr3MPT->AddProperty("ABSLENGTH",PhotonEnergy,LaBr3AbsorptionLength,
						  nEntries);
	
	G4double ScintEnergy[nEntries] = {3.26*eV,3.44*eV};
	G4double ScintFast[nEntries] = {1.0,1.0};
	
	LaBr3MPT->AddProperty("FASTCOMPONENT",ScintEnergy,ScintFast,nEntries);
	
	LaBr3MPT->AddConstProperty("SCINTILLATIONYIELD",63./keV); 
	LaBr3MPT->AddConstProperty("RESOLUTIONSCALE",1.);
	LaBr3MPT->AddConstProperty("FASTTIMECONSTANT",20.*ns);
	LaBr3MPT->AddConstProperty("YIELDRATIO",1.);
	
	LaBr3->SetMaterialPropertiesTable(LaBr3MPT);
		
	// Quartz
	
	G4double QuartzRefractionIndex[nEntries] = {1.47,1.47};
	
	G4double QuartzAbsorptionLength[nEntries] = {3.0*cm,3.0*cm};
	
	G4MaterialPropertiesTable* QuartzMPT = new G4MaterialPropertiesTable();
	
	QuartzMPT->AddProperty("RINDEX",PhotonEnergy,QuartzRefractionIndex,
						   nEntries);
	QuartzMPT->AddProperty("ABSLENGTH",PhotonEnergy,QuartzAbsorptionLength,
						   nEntries);
	
	Quartz->SetMaterialPropertiesTable(QuartzMPT);
	
	// K2CsSb (Bialcali Photocathode)
	
	G4double K2CsSbRefractionIndex[nEntries] = {1.47,1.47};
	
	G4double K2CsSbAbsorptionLength[nEntries] = {1.0E-9*m,1.0E-9*m};
	
	G4MaterialPropertiesTable* K2CsSbMPT = new G4MaterialPropertiesTable();
	
	K2CsSbMPT->AddProperty("RINDEX",PhotonEnergy,K2CsSbRefractionIndex,
						   nEntries);
	K2CsSbMPT->AddProperty("ABSLENGTH",PhotonEnergy,K2CsSbAbsorptionLength,
						   nEntries);
	
	K2CsSb->SetMaterialPropertiesTable(K2CsSbMPT);
	
	// Vacuum
	
	G4double vacRefractionIndex[nEntries] = {1.0,1.0};
	
	
	G4MaterialPropertiesTable* vacMPT = new G4MaterialPropertiesTable();
	vacMPT->AddProperty("RINDEX",PhotonEnergy,vacRefractionIndex,
						nEntries);
	
	
	Vacuum->SetMaterialPropertiesTable(vacMPT);
	
	//------------------------------------------------------
	// Detector geometry
	//------------------------------------------------------
	
	//     
	// World
	//
	
	G4double WorldSize= 30.*cm;
	
	G4Box* 
    solidWorld = new G4Box("World",		       	                  //its name
						   WorldSize/2,WorldSize/2,WorldSize/2);  //its size
	
	G4LogicalVolume* 
    logicWorld = new G4LogicalVolume(solidWorld,      	//its solid
									 Vacuum,	        //its material
									 "World");		    //its name
	
	G4VPhysicalVolume* 
    physiWorld = new G4PVPlacement(0,			    //no rotation
								   G4ThreeVector(),	//at (0,0,0)
								   "World",		    //its name
								   logicWorld,		//its logical volume
								   NULL,		    //its mother  volume
								   false,	       	//no boolean operation
								   0);			    //copy number
	
	
	//
	// Detector 
	//
	
	G4double ScintHalfLength =1.5*cm;
	G4double ScintRadius = 2.*cm;
	
    G4double ReflectorThickness = 0.5*mm;
	G4double ReflectorHalfLength = ScintHalfLength+ReflectorThickness;
	G4double ReflectorRadius = ScintRadius+ReflectorThickness;
	
	G4double PMTWindowHalfLength = 1.0*mm;
	G4double PMTWindowRadius = 1.8*cm;
	
	G4double CathodeHalfLength = 0.005*mm;
	G4double CathodeRadius =1.9*cm;
	
	G4double StartPhi = 0.*deg;
	G4double DeltaPhi = 360.*deg;
	
	
	// Reflector
	
	G4Tubs* solidReflector = new G4Tubs("Reflector",0.*cm,
										ReflectorRadius,ReflectorHalfLength,StartPhi,DeltaPhi);
	
	G4LogicalVolume* logicReflector = new G4LogicalVolume(solidReflector,MgO,
														  "Reflector");
	
	G4ThreeVector positionReflector = G4ThreeVector(0.*cm,0.*cm,0.*cm);
	
	G4VPhysicalVolume* physiReflector = new G4PVPlacement(0,positionReflector,
														  "Reflector",logicReflector,
														  physiWorld,false,0);
	
	//Crystal
	
	G4Tubs* solidCrystal = new G4Tubs("Crystal", 0.*cm, ScintRadius,
									  ScintHalfLength,StartPhi,DeltaPhi);
	
	G4LogicalVolume* logicCrystal = new G4LogicalVolume(solidCrystal,LaBr3,
														"Crystal");
	
	G4ThreeVector positionCrystal = G4ThreeVector(0.*cm,0.*cm,
                                                  ReflectorThickness);
	
	G4VPhysicalVolume* physiCrystal = new G4PVPlacement(0,positionCrystal,
														"Crystal",logicCrystal,
														physiReflector,false,0);
	
	
	// PMT window
	
	G4Tubs* solidPMTWindow = new G4Tubs("PMTWindow",0.*cm,PMTWindowRadius,
										PMTWindowHalfLength,StartPhi,DeltaPhi);
	
	G4LogicalVolume* logicPMTWindow = new G4LogicalVolume(solidPMTWindow,
														  Quartz,"PMTWindow");
	
	G4ThreeVector positionPMTWindow = G4ThreeVector(0.*cm,0.*cm,
													ReflectorHalfLength+PMTWindowHalfLength);
	
	G4VPhysicalVolume* physiPMTWindow = new G4PVPlacement(0,positionPMTWindow,
														  "PMTWindow",logicPMTWindow,
														  physiWorld,false,0);
	
	// Photocathode
	
	G4Tubs* solidCathode = new G4Tubs("Cathode",0.*cm,CathodeRadius,
									  CathodeHalfLength,StartPhi,DeltaPhi);
	
	G4LogicalVolume* logicCathode = new G4LogicalVolume(solidCathode,
														K2CsSb,"Cathode");
	
	G4ThreeVector positionCathode = G4ThreeVector(0.*cm,0.*cm,
												  ReflectorHalfLength+2.*PMTWindowHalfLength
												  +CathodeHalfLength);
	
	G4VPhysicalVolume* physiCathode = new G4PVPlacement(0,positionCathode,
														"Cathode",logicCathode,
														physiWorld,false,0);
	
	
	//------------------------------------------------------
	// Surfaces and boundary processes
	//------------------------------------------------------
	
	// Reflector - sintillator surface 
	
	G4OpticalSurface* OpRefCrySurface = 
	new G4OpticalSurface("RefCrySurface");
	
	OpRefCrySurface->SetType(dielectric_metal);
	OpRefCrySurface->SetModel(glisur);
	OpRefCrySurface->SetFinish(polished);
	
	G4LogicalBorderSurface* RefCrySurface = 
    new G4LogicalBorderSurface("RefCrySurface",physiCrystal,
							   physiReflector,OpRefCrySurface);
	
	
	// Scintillator - PMT window surface 
	
	G4OpticalSurface* OpCryPMTWinSurface = 
	new G4OpticalSurface("CryPMTWinSurface");
	
	OpCryPMTWinSurface->SetType(dielectric_dielectric);
	OpCryPMTWinSurface->SetModel(glisur);
	OpCryPMTWinSurface->SetFinish(polished);
	
	G4LogicalBorderSurface* CryPMTWinSurface = 
    new G4LogicalBorderSurface("CryPMTWinSurface",physiCrystal,physiPMTWindow,
							   OpCryPMTWinSurface);
	
	// PMT window - photocathode surface
	
	G4OpticalSurface* OpPMTWinCathSurface = new G4OpticalSurface("PMTWinCathSurface");
	
	OpPMTWinCathSurface->SetType(dielectric_dielectric);
	OpPMTWinCathSurface->SetModel(glisur);
	OpPMTWinCathSurface->SetFinish(polished);
	
	G4LogicalBorderSurface* PMTWinCathSurface = 
    new G4LogicalBorderSurface("CathodeSurface",physiPMTWindow,physiCathode,
							   OpPMTWinCathSurface);
	
	
	//------------------------------------------------------
	// visualization attributes
	//------------------------------------------------------
	
	logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
	
	//Green color for scintillator crystal
	G4VisAttributes* Att1= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
	logicCrystal->SetVisAttributes(Att1);
	
	//Yellow color for reflector
	G4VisAttributes* Att2= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
	logicReflector->SetVisAttributes(Att2);
	
	//Blue color for PMT window
	G4VisAttributes* Att3= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
	logicPMTWindow->SetVisAttributes(Att3);
	
	//White color for the absorber photocathode
	G4VisAttributes* Att4= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	logicCathode->SetVisAttributes(Att4);
    
	//
	// always return the physical World
	//
	
	return physiWorld;
}
