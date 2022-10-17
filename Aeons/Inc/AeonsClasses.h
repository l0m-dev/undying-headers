#include "Engine.h"
//#include "EngineClasses.h"

#if _MSC_VER
#pragma pack (push,4)
#endif

#ifndef AEONS_API
#define AEONS_API DLL_IMPORT
#endif

struct ImpactSoundParams
{
    float MaxVolume;
    float MinVolume;
    float Radius;
    float MaxPitch;
    float MinPitch;
    USound* Sound_1;
    USound* Sound_2;
    USound* Sound_3;
};
struct PlayerModifier
{
    USound* ActivateSound;
    USound* DeactivateSound;
    USound* EffectSound;
    INT ManaCost;			// Amount of mana used to activate this modifier
    INT CastingLevel;		// Amplitude this modifier was activated at
    FLOAT refreshRate;		// 

    BITFIELD bActive:1;
};

class AEONS_API AAeonsWeapon : public AWeapon
{
public:
    INT NumRicoSounds;
    USound* RicoSounds[8];
    INT ClipCount;
    DWORD bReloadable : 1;
    INT hitdamage;
    FLOAT MomentumTransfer; //For instant hit weapons
    DWORD bDestroyOnEmpty : 1;
    FString ExpireMessage; // Message shown when no weapon ammo and bDestroyOnEmpty
    DWORD bDestroyMe : 1; //we've expired and dying
    DWORD bWaterFire : 1; //can fire under water
    DWORD bAltWaterFire : 1; //can alt fire under water
    FLOAT ReloadTime; // how long do I take to reload?

    USound* OpenSound;
    USound* CloseSound;
    class UDecal* ExplosionDecal;

    UMesh* AltAmmoMesh;

    FLOAT HeadShotMult; // Damage Multiplier for a head shot
    FName ThirdPersonJointName;
    FLOAT RefireMult;

    //weapon new multiplayer variables
    DWORD bCanClientFire : 1;
    DWORD bForceFire : 1;
    FLOAT FireTime; //used to synch server and client firing up
    FLOAT FireStartTime;
    UMesh* TempMesh;
    DWORD bFiring : 1;

    DECLARE_CLASS(AAeonsWeapon, AWeapon, 0 | CLASS_NativeReplication, Engine)
    NO_DEFAULT_CONSTRUCTOR(AAeonsWeapon)
};

/*
class AEONS_API ASpell : public AInventory
{
public:
    INT castingLevel; // casting level of the spell - the base casting level of the spell
    INT LocalCastingLevel; // casting level of the spell after modifications from the ghelziabahr stone.

    INT amplitudeBonus; // amplitude bunus to the castingLevel (i.e. from the Ghelzibahar Stone)
    INT manaCostPerLevel[6];
    INT damagePerLevel[6];

    DWORD bInControl : 1; // If true, then this is the currently active and rendered spell
    DWORD bPassControl : 1; // If true, then fire the other currently active spell (Attack or Defense)
    FLOAT MaxTargetRange; // Maximum distance to target.
    DWORD bPointing : 1; // Indicates spell is being pointed
    DWORD bInstantHit : 1; // If true, instant hit rather than projectile firing
    DWORD bWaterFire : 1; //can fire under water
    DWORD bWarnTarget : 1; // When firing projectile, warn the target
    DWORD bSpellUp : 1; // Used in Active State
    DWORD bChangeSpell : 1; // Used in Active State
    DWORD bSplashDamage : 1; // used by bot AI
    DWORD bRecommendSplashDamage : 1; //if true, bot preferentially tries to use splash damage
    // rather than direct hits
    DWORD bOwnsCrosshair : 1; // this spell is responsible for drawing its own crosshair (in its postrender function)
    DWORD bHideSpell : 1; // if true, spell is not rendered

    FVector FireOffset; // Offset from drawing location for projectile/trace start
    class AActor* ProjectileClass; // fix projectile
    FName MyDamageType; //set from the projectile's stats
    FLOAT ProjectileSpeed; //set from the projectile's stats
    FLOAT AimError; // Aim Error for bots (note this value doubled if instant hit )
    FLOAT ShakeMag;
    FLOAT ShakeTime;
    FLOAT ShakeVert;
    FLOAT AIRating;
    FLOAT RefireRate; //likelyhood that AI will immed. fire again
    FLOAT Accuracy; //inherent spell accuracy -for rapid fire spread

    FLOAT ExtraFireDelay; //sleep for this amount after fire anim
    FLOAT ExtraSpellUpDelay; //sleep for this amount after spell up anim
    FLOAT ExtraSpellDownDelay; //sleep for this amount after spell down anim

    //-----------------------------------------------------------------------------
    // USound* Assignments
    USound* FireSound;
    USound* FizzleSound;
    USound* SelectSound;
    USound* Misc1Sound;
    USound* Misc2Sound;
    USound* Misc3Sound;

    FString MessageNoMana;
    FString DeathMessage;

    FRotator AdjustedAim;
    DWORD bFiring : 1;

    //-----------------------------------------------------------------------------
    // Muzzle Flash
    // spell is responsible for setting and clearing bMuzzleFlash whenever it wants the
    // MFUTexture* drawn on the canvas (see RenderOverlays() )
    DWORD bSetFlashTime : 1;
    DWORD bDrawMuzzleFlash : 1;
    BYTE bMuzzleFlash;
    FLOAT FlashTime;
    FLOAT MuzzleScale, FlashY, FlashO, FlashC, FlashLength;
    INT FlashS; // size of (square) texture/2
    UTexture* MFTexture;
    UTexture* MuzzleFlare;
    FLOAT FlareOffset;

    DECLARE_CLASS(ASpell, AInventory, 0 | CLASS_NativeReplication, Engine)
    NO_DEFAULT_CONSTRUCTOR(ASpell)
        //#include "AWeapon.h"
};
*/

class AEONS_API ABookJournalBase : public AInventory
{
public:
    INT NumUnreadJournals;
    class AActor* NewestUnread; // fix JournalEntry

    INT NumJournals;
    INT CurrentJournalIndex;
    INT FirstJournalId;

    INT JournalRead[150];
    FString JournalNames[150];
    class AActor* Journals[150]; // fix travel  ?, JournalEntry

    // someone wants us to display a specific JournalEntry class in the book.
    class AActor* RequestedEntryClass; // fix JournalEntry

    DECLARE_CLASS(ABookJournalBase, AInventory, 0 | CLASS_NativeReplication, Engine)
    NO_DEFAULT_CONSTRUCTOR(ABookJournalBase)
};

class AEONS_API AAeonsPlayer : public APlayerPawn
{
public:
    struct ShalasModifier : PlayerModifier
    {
        DWORD bHUDEffect:1;
        FLOAT absorbManaPct;
        FLOAT Str;			// strength of HUD Effect
        FVector Col;			// color of HUD effect
        FVector AdjLoc;
        AAeonsPlayer* Player;
    };

    struct OSMModifier : PlayerModifier
    {
        char* DisplayedMessage;
        DWORD bServerMessage:1;
    };
    
    struct FlightModifier : PlayerModifier
    {
        USound* FlyingSound;
        USound* SputterSound;
        FLOAT Fuel;
        BYTE InitialVolume;
    };
    
    USound* Drown;
    USound* BreathAgain;
    USound* Footstep1;
    USound* Footstep2;
    USound* Footstep3;
    USound* Swim1;
    USound* Swim2;
    USound* Swim3;
    USound* HitSound3;
    USound* HitSound4;
    USound* Die2;
    USound* Die3;
    USound* Die4;
    USound* GaspSound;
    USound* UWHit1;
    USound* UWHit2;
    USound* LandGrunt;
    USound* UnderWater;
    // normally I would just AmbientSound, but of course other effects will probably need it.
    // So I'll just play the sound, save the ID and stopsound when the player leaves the water
    INT UnderWaterSoundID;
    FLOAT SoundDuration;

    class AActor* OverlayActor; // for rendering overlays outside the player class.

    DWORD bDoubleShotgun:1;
    // DWORD bAmplifySpell:1;				// should the next spell to be fired amplify one level?
    DWORD bLanternOn:1;
    // Spell effects on the player
    DWORD bWeaponSound:1; // players conventional weapon makes a sound? Incantation of Silence Spell affects this
    DWORD bMagicSound:1; // players cast spells make sounds? Incantation of Silence Spell affects this
    FLOAT refireMultiplier; // multiplier for the refire rate of weapons and spells - the Haste modifier affects this
    FLOAT speedMultiplier;
    DWORD bRenderWeapon:1;

    // WizardEye Support
    class AActor* wizEye; // Wizard eye projectile - to look through. fix WizardEye_proj
    DWORD bWizardEye:1; // looking through wizard eye right now
    DWORD bPhoenix:1;

    // Player Modifiers ---------------------------------------------------------------------------
    PlayerModifier* WardMod; // Ward Modifier*
    PlayerModifier* HasteMod; // Haste Modifier*
    PlayerModifier* SphereOfColdMod; // Sphere of Cold Modifier*
    PlayerModifier* MindshatterMod; // Mindshatter Modifier*
    PlayerModifier* PhaseMod; // Phase Modifier*
    PlayerModifier* SilenceMod; // Incantation of Silence Modifier*
    PlayerModifier* DispelMod; // Dispel Magic Modifier*
    PlayerModifier* ShieldMod; // Shield Modifier*
    ShalasModifier* ShalasMod; // Shalas Vortex Modifier*
    PlayerModifier* FireFlyMod; // Firefly Modifier*
    PlayerModifier* ManaMod; // Mana Modifier*
    PlayerModifier* ScryeMod; // Scrye Modifier*
    PlayerModifier* HealthMod; // Health Modifier*
    PlayerModifier* SoundMod; // Sound Modifier*
    PlayerModifier* StealthMod; // Stealth Modifier*
    PlayerModifier* PlayerDamageMod; // Player Damage Physics Modifier*
    PlayerModifier* GameStateMod; // Game State tracking Modifier*
    PlayerModifier* InvokeMod; // Invoke Modifier*
    PlayerModifier* RainMod; // Rain & Snow Modifier*
    PlayerModifier* FrictionMod; // Friction Modifier*
    PlayerModifier* SlothMod; // Slowing Modifier*
    PlayerModifier* SlimeMod; // Slime Modifier*
    OSMModifier* OSMMod; // OnScreen Message Modifier* fix

    // Game Events	-------------------------------------------------------------------------------

    DWORD bLizbethDead:1;
    DWORD bAmbroseDead:1;
    DWORD bJeremiahTalk1:1;
    DWORD bJeremiahTalk2:1;
    DWORD bJeremiahDead:1;
    DWORD bAaronDead:1;
    DWORD bBethanyDead:1;
    DWORD bKeisingerDead:1;
    DWORD bReturnfromPiratesCove:1;
    DWORD bReturnfromOneiros:1;
    DWORD bRevenant:1;
    DWORD bInnercourtyard_silverbullets1:1;
    DWORD bInnercourtyard_silverbullets2:1;
    DWORD bInnercourtyard_phosphorus1:1;
    DWORD bInnercourtyard_phosphorus2:1;
    DWORD bInnercourtyard_phosphorus3:1;
    DWORD bInnercourtyard_health1:1;
    DWORD bInnercourtyard_health2:1;
    DWORD bInnercourtyard_manawell:1;
    DWORD bInnercourtyard_arcanewhorl:1;
    DWORD bNorthwinglower_kitchen_health:1;
    DWORD bNorthwinglower_brewery_health:1;
    DWORD bNorthwinglower_diningroom_amplifier:1;
    DWORD bNorthwinglower_basement_amplifier:1;
    DWORD bNorthwinglower_basement_bullets1:1;
    DWORD bNorthwinglower_basement_bullets2:1;
    DWORD bNorthwinglower_basement_phosphorus1:1;
    DWORD bNorthwinglower_basement_phosphorus2:1;
    DWORD bNorthwinglower_basement_molotov1:1;
    DWORD bNorthwinglower_basement_molotov2:1;
    DWORD bNorthwinglower_basement_molotov3:1;
    DWORD bNorthwinglower_basement_molotov4:1;
    DWORD bNorthwinglower_dayroom_health:1;
    DWORD bNorthwingupper_servantsq_health:1;
    DWORD bNorthwingupper_servantsq_bullets:1;
    DWORD bNorthwingupper_servantsq_shotgunammo:1;
    DWORD bNorthwingupper_servantsq_molotov1:1;
    DWORD bNorthwingupper_servantsq_molotov2:1;
    DWORD bNorthwingupper_aaronsroom_health:1;
    DWORD bNorthwingupper_aaronsroom_molotov1:1;
    DWORD bNorthwingupper_aaronsroom_molotov2:1;
    DWORD bNorthwingupper_aaronsroom_molotov3:1;
    DWORD bNorthwingupper_aaronsroom_molotov4:1;
    DWORD bNorthwingupper_aaronsroom_molotov5:1;
    DWORD bWestWing_Conservatory_Health:1;
    DWORD bWestWing_Conservatory_ServantKey1:1;
    DWORD bWestWing_Conservatory_Amplifier:1;
    DWORD bWestWing_SmokingRoom_Health:1;
    DWORD bWestWing_HuntingRoom_Bullets1:1;
    DWORD bWestWing_HuntingRoom_Bullets2:1;
    DWORD bWestWing_Jeremiah_Silver1:1;
    DWORD bWestWing_Jeremiah_Silver2:1;
    DWORD bWestWing_Jeremiah_Silver3:1;
    DWORD bWestWing_Jeremiah_Silver4:1;
    DWORD bWestWing_Jeremiah_bullets:1;
    DWORD bWestWing_Jeremiah_health:1;
    DWORD bGreatHall_attic_amplifier:1;
    DWORD bCentralUpper_Lizbeth_Health:1;
    DWORD bCentralUpper_Lizbeth_Poetry:1;
    DWORD bCentralUpper_Bethany_Diary:1;
    DWORD bCentralUpper_Study_JoeNotes:1;
    DWORD bCentralUpper_Study_EtherTrap1:1;
    DWORD bCentralUpper_Study_EtherTrap2:1;
    DWORD bCentralUpper_TowerStairs_Gate:1;
    DWORD bCentralLower_SunRoom_BethsLetters:1;
    DWORD bCentralLower_Tower_amplifier:1;
    DWORD bZagnutz:1;
    DWORD bCentralLower_TowerAccess:1;
    DWORD bEastWingLower_Nursery_Health:1;
    DWORD bEastWingLower_Nursery_ServantDiary:1;
    DWORD bEastWingLower_BackStairs_Amplifier:1;
    DWORD bEastWingUpper_UpperBackAccess:1;
    DWORD bWidowsWatch_SmallGardenAccess:1;
    DWORD bGardens_ToolShop_Health1:1;
    DWORD bGardens_ToolShop_Health2:1;
    DWORD bGardens_ToolShop_Dynamite1:1;
    DWORD bGardens_ToolShop_Dynamite2:1;
    DWORD bGardens_ToolShop_Dynamite3:1;
    DWORD bGardens_ToolShop_Dynamite4:1;
    DWORD bGardens_ToolShop_Dynamite5:1;
    DWORD bGardens_ToolShop_Dynamite6:1;
    DWORD bGardens_Greenhouse_phosphorus1:1;
    DWORD bGardens_Greenhouse_phosphorus2:1;
    DWORD bGardens_Greenhouse_phosphorus3:1;
    DWORD bGardens_Greenhouse_phosphorus4:1;
    DWORD bGardens_Greenhouse_phosphorus5:1;
    DWORD bGardens_Greenhouse_phosphorus6:1;
    DWORD bGardens_Greenhouse_phosphorus7:1;
    DWORD bGardens_Greenhouse_Health:1;
    DWORD bGardens_Greenhouse_BethanyKey:1;
    DWORD bGardens_Well_Amplifier:1;
    DWORD bInnercourtyard_BalconyDoorAccess:1;
    DWORD bGreatHall_attic_bullets1:1;
    DWORD bGreatHall_attic_bullets2:1;
    DWORD bGreatHall_Shotgunshells1:1;
    DWORD bGreatHall_Shotgunshells2:1;
    DWORD bGreatHall_Health1:1;
    DWORD bGreatHall_Health2:1;
    DWORD bGreatHall_Molotov1:1;
    DWORD bGreatHall_Molotov2:1;
    DWORD bGreatHall_Molotov3:1;
    DWORD bGreatHall_Molotov4:1;
    DWORD bGreatHall_AtticAccess:1;
    DWORD bInnercourtyard_amplifier:1;
    DWORD bInnercourtyard_AaronsRoomKey:1;
    DWORD bInnercourtyard_molotov1:1;
    DWORD bInnercourtyard_molotov2:1;
    DWORD bInnercourtyard_molotov3:1;
    DWORD bTowerRun_Inhabitants_amplifier:1;
    DWORD bTowerRun_dynamite1:1;
    DWORD bTowerRun_dynamite2:1;
    DWORD bTowerRun_dynamite3:1;
    DWORD bTowerRun_dynamite4:1;
    DWORD bTowerRun_Health:1;
    DWORD bTowerRun_Amplifier:1;
    DWORD bTowerRun_TowerAccess:1;
    DWORD bChapel_etherTrap1:1;
    DWORD bChapel_etherTrap2:1;
    DWORD bChapel_Health1:1;
    DWORD bChapel_Health2:1;
    DWORD bChapel_Paper:1;
    DWORD bChapel_Tome:1;
    DWORD bChapel_amplifier:1;
    DWORD bChapel_PriestKey:1;
    DWORD bSedgewickConversation:1;
    DWORD bKiesingerConversation:1;
    DWORD bEastWingUpper_Guest_Health1:1;
    DWORD bEastWingUpper_Guest_Health2:1;
    DWORD bEastWingUpper_Ambrose_Health:1;
    DWORD bEastWingUpper_Ambrose_Journal:1;
    DWORD bEastWingUpper_Ambrose_Pirate:1;
    DWORD bEastWingUpper_Ambrose_Phosphorus1:1;
    DWORD bEastWingUpper_Ambrose_Phosphorus2:1;
    DWORD bEastWingUpper_Ambrose_Phosphorus3:1;
    DWORD bEastWingUpper_Keisinger_Journal:1;
    DWORD bEastWingUpper_Office_Evaline:1;
    DWORD bEastWingUpper_ReadingRoom_Health1:1;
    DWORD bEastWingUpper_ReadingRoom_Health2:1;
    DWORD bEastWingUpper_Bar_molotov1:1;
    DWORD bEastWingUpper_Bar_molotov2:1;
    DWORD bEastWingUpper_Bar_molotov3:1;
    DWORD bEastWingUpper_Lounge_ShotgunShells:1;
    DWORD bEastWingUpper_Hallway_Amplifier:1;
    DWORD bVisitAaronsStudio:1;
    DWORD bCentralUpper_WidowsWatchKey:1;
    DWORD bCentralUpper_Josephsconcern:1;
    DWORD bEntranceHall_JoesRoom_Joenotes:1;
    DWORD bEntranceHall_EvasRoom_EvalinesDiary:1;
    DWORD bNorthWIngUpper_Aaron_amplifier:1;
    DWORD bLearnofPiratesCove:1;
    DWORD bWestWing_Jeremiah_StudyKey:1;
    DWORD bNorthWIngUpper_Aaron_BethanyGate:1;
    DWORD bCentralUpper_Study_Health:1;
    DWORD bMonasteryPastFinished:1;
    DWORD bAmbrosesRoom:1;
    DWORD bVisitStandingStones:1;
    DWORD bFlightEnabled:1;
    DWORD bAfterChapel:1;
    DWORD bKiesingerDead:1;
    DWORD bChandelierFell:1;
    DWORD bBethanyTransformed:1;
    DWORD bAmplifierFound:1;
    DWORD bOracleReturn:1;
    DWORD bWhorlFound:1;
    DWORD bEtherFound:1;
    DWORD bPostShrine:1;
    DWORD bManaWellFound:1;
    DWORD bChapel_EtherTrap3:1;
    DWORD bChapel_EtherTrap4:1;
    DWORD bChapel_Bullets:1;
    DWORD bEnteredJeremiahsRoom:1;

    // --------------------------------------------------------------------------------------------

    DWORD bLogGameState:1;
    
    class AActor* MasterCamPoint; //fix MasterCameraPoINT

    // Modifier states
    DWORD bWardActive:1;
    DWORD bHasteActive:1;
    DWORD bColdActive:1;
    DWORD bMindActive:1;
    DWORD bSilenceActive:1;
    DWORD bDispelActive:1;
    DWORD bShieldActive:1;
    DWORD bShalasActive:1;
    DWORD bFireFlyActive:1;
    DWORD bScryeActive:1;
    DWORD bPhaseActive:1;
    DWORD bSlothActive:1;
    DWORD bSlimeActive:1;

    FRotator CamDebugRotDiff;
    FVector CamDebugPosDiff;
    FVector LookAt1;
    FVector LookAt2;
    FVector LookDir;
    FLOAT pDist;

    class AActor* InvDisplayMan;

    class AACtor* VoiceType;

    FLOAT CrossHairOffsetX, CrossHairOffsetY, crossHairScale; // offsets for the crosshair
    DWORD bDrawCrosshair:1;

    // book item
    ABookJournalBase* Book;

    // debug HUD flags
    DWORD bDrawPawnName:1; // draw the name of the Pawn the cross hair is over
    DWORD bDrawActorName:1; // draw the name of the AActor* the cross hair is over
    DWORD bDrawStealth:1; // draw stealth info to the hud.

    DWORD bDrawInvList:1; // Draw the Inventory List?
    DWORD bDrawDebugHUD:1; // Draw debug items in the hud

    FLOAT NoManaFlashTime;

    DWORD bAllowSelectionHUD:1;
    enum ESelectMode : unsigned char {
        SM_None,
        SM_Weapon,
        SM_AttSpell,
        SM_DefSpell
    } SelectMode;

    FLOAT SelectTimer; // max double click interval for select object
    FLOAT SelectTime;
    FLOAT SelectTimePSX2;
    DWORD bTryingSelect:1;
    DWORD bSelectObject:1; //set when in selection is active

    FLOAT FireDefSpellHeldTime; //tracks length of bFireAttSpell being held down
    FLOAT FireAttSpellHeldTime; //tracks length of bFireAttSpell being held down
    FLOAT FireHeldTime; //tracks length of bFire being held down
    FLOAT JumpHeldTime; //tracks length of Jump being held down
    FlightModifier* Flight;

    AAeonsWeapon* ClientPending;
    AAeonsWeapon* OldClientWeapon;
    DWORD bNeedActivate:1;
    INT WeaponUpdate;

    // main mana vars moved to pawn
    DWORD bUseMana:1; // for debugging - set to false to not decrement mana
    INT ManaWellsFound; //For selecting anim for spell hand
    INT ManaWhorlsFound; //For selecting anim for spell hand

    /////////////////////////////////////////////////////////////
    INT FavWeapon1; //favorite weapon #1
    INT FavWeapon2;
    BITFIELD FavWeaponToggle:1; //decides next favorite to be overwritten
    INT FavAttSpell1; //favorite attack spell #1
    INT FavAttSpell2;
    BITFIELD FavAttSpellToggle:1; //decides next favorite to be overwritten
    INT FavDefSpell1; //favorite defense spell #1
    INT FavDefSpell2;
    BITFIELD FavDefSpellToggle:1; //decides next favorite to be overwritten

    /////////////////////////////////////////////////////////////
    DWORD bTryingScroll:1; // like bTryingSelect, except for the PS2 scrolling
    DWORD bScrollObject:1; // have we actually entered the PS2 scrolling phase
    AInventory* SelectedInvPSX2;

    struct DefUserSettingsPSX2 // saves settings from the options screen
    {
        BYTE SoundVolumePSX2;
        BYTE MusicVolumePSX2;
        BYTE ControllerConfigPSX2;
        BITFIELD EasyAimPSX2:1;
        BITFIELD InvertLookPSX2:1;
        BITFIELD VibrationPSX2:1;
    } UserSettingsPSX2;

    FLOAT ScrollTimerPSX2; // times scrolling for repetition
    FLOAT ScrollDelayPSX2; // time between scrolling repeats

    /////////////////////////////////////////////////////////////

    DWORD bRequestedShot:1;
    CHAR* SavePath;

    FLOAT fDecision;
    FVector LastStepLocation;
    FVector StepDisplacement;

    class AActor* EMod; // fix

    class AActor* wind; // fix

    BYTE Objectives[100];
    CHAR ObjectivesText[100];

    DWORD bShowScryeHint:1;

    DECLARE_CLASS(AAeonsPlayer, APlayerPawn, 0 | CLASS_Config | CLASS_NativeReplication, Engine)//CLASS_Config
    NO_DEFAULT_CONSTRUCTOR(AAeonsPlayer)
};
