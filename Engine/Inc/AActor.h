/*=============================================================================
	AActor.h.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

	// Constructors.
	AActor() {}
	void Destroy();

	// UObject interface.
	virtual INT* GetOptimizedRepList( BYTE* InDefault, FPropertyRetirement* Retire, INT* Ptr, UPackageMap* Map, INT NumReps );
	virtual UBOOL ShouldDoScriptReplication() {return 1;}
	virtual UBOOL NoVariablesToReplicate(AActor *OldVer) {return 0;};
	virtual UBOOL CheckRecentChanges() {return 0;};
	virtual FLOAT UpdateFrequency(AActor *Viewer, FVector &ViewDir, FVector &ViewPos);
	void ProcessEvent( UFunction* Function, void* Parms, void* Result=NULL );
	void ProcessState( FLOAT DeltaSeconds );
	UBOOL ProcessRemoteFunction( UFunction* Function, void* Parms, FFrame* Stack );
	void ProcessDemoRecFunction( UFunction* Function, void* Parms, FFrame* Stack );
	void Serialize( FArchive& Ar );
	void InitExecution();
	void PostEditChange();
	void PostLoad();

	// AActor interface.
	class ULevel* GetLevel() const;
	class APlayerPawn* GetPlayerPawn() const;
	UBOOL IsPlayer() const;
	UBOOL IsOwnedBy( const AActor *TestOwner ) const;
	FLOAT WorldSoundRadius() const {return 25.f * ((INT)SoundRadius+1);}
	FLOAT WorldVolumetricRadius() const {return 25.f * ((INT)VolumeRadius+1);}
	UBOOL IsBlockedBy( const AActor* Other ) const;
	UBOOL IsInZone( const AZoneInfo* Other ) const;
	UBOOL IsBasedOn( const AActor *Other ) const;
	virtual FLOAT GetNetPriority( AActor* Sent, FLOAT Time, FLOAT Lag );
	virtual FLOAT WorldLightRadius() const {return 25.f * ((INT)LightRadius+1);}
	virtual UBOOL Tick( FLOAT DeltaTime, enum ELevelTick TickType );
	virtual void PostEditMove() {}
	virtual void PreRaytrace() {}
	virtual void PostRaytrace() {}
	virtual void Spawned() {}
	virtual void PreNetReceive();
	virtual void PostNetReceive();
	virtual UTexture* GetSkin( INT Index );
	virtual FMeshAnimSeq* GetAnim( FName SequenceName );
	virtual FCoords ToLocal() const
	{
		return GMath.UnitCoords / Rotation / Location;
	}
	virtual FCoords ToWorld() const
	{
		return GMath.UnitCoords * Location * Rotation;
	}
	FLOAT LifeFraction()
	{
		return Clamp( 1.f - LifeSpan / GetClass()->GetDefaultActor()->LifeSpan, 0.f, 1.f );
	}
	FVector GetCylinderExtent() const {return FVector(CollisionRadius,CollisionRadius,CollisionHeight);}
	AActor* GetTopOwner();
	UBOOL IsPendingKill() {return bDeleteMe;}

	// AActor collision functions.
	UPrimitive* GetPrimitive() const;
	UBOOL IsOverlapping( const AActor *Other ) const;

	// AActor general functions.
	void BeginTouch(AActor *Other);
	void EndTouch(AActor *Other, UBOOL NoNotifySelf);
	void SetOwner( AActor *Owner );
	UBOOL IsBrush()       const;
	UBOOL IsStaticBrush() const;
	UBOOL IsMovingBrush() const;
	UBOOL IsAnimating() const
	{
		#ifdef UNREAL_ANIM
		return
			(AnimSequence!=NAME_None)
		&&	(AnimFrame>=0 ? AnimRate!=0.f : TweenRate!=0.f);
		#else
		return false;
		#endif
	}
	void SetCollision( UBOOL NewCollideActors, UBOOL NewBlockActors, UBOOL NewBlockPlayers);
	void SetCollisionSize( FLOAT NewRadius, FLOAT NewHeight );
	void SetBase(AActor *NewBase, int bNotifyActor=1);
	FRotator GetViewRotation();
	FBox GetVisibilityBox();

	// AActor audio.
	void MakeSound( USound *Sound, FLOAT Radius=0.f, FLOAT Volume=1.f, FLOAT Pitch=1.f );
	void CheckHearSound(APawn* Hearer, INT Id, USound* Sound, FVector Parameters, FLOAT RadiusSquared);

	// Physics functions.
	void setPhysics(BYTE NewPhysics, AActor *NewFloor = NULL);
	void FindBase();
	virtual void performPhysics(FLOAT DeltaSeconds);
	void physProjectile(FLOAT deltaTime, INT Iterations);
	void processHitWall(FVector HitNormal, AActor *HitActor);
	void processLanded(FVector HitNormal, AActor *HitActor, FLOAT remainingTime, INT Iterations);
	void physFalling(FLOAT deltaTime, INT Iterations);
	void physRolling(FLOAT deltaTime, INT Iterations);
	void physicsRotation(FLOAT deltaTime);
	int fixedTurn(int current, int desired, int deltaRate); 
	inline void TwoWallAdjust(FVector &DesiredDir, FVector &Delta, FVector &HitNormal, FVector &OldHitNormal, FLOAT HitTime)
	{
		guard(AActor::TwoWallAdjust);

		if ((OldHitNormal | HitNormal) <= 0) //90 or less corner, so use cross product for dir
		{
			FVector NewDir = (HitNormal ^ OldHitNormal);
			NewDir = NewDir.SafeNormal();
			Delta = (Delta | NewDir) * (1.f - HitTime) * NewDir;
			if ((DesiredDir | Delta) < 0)
				Delta = -1 * Delta;
		}
		else //adjust to new wall
		{
			Delta = (Delta - HitNormal * (Delta | HitNormal)) * (1.f - HitTime); 
			if ((Delta | DesiredDir) <= 0)
				Delta = FVector(0,0,0);
		}
		unguard;
	}
	void physPathing(FLOAT DeltaTime);
	void physMovingBrush(FLOAT DeltaTime);
	void physTrailer(FLOAT DeltaTime);
	int moveSmooth(FVector Delta);

	// AI functions.
	void CheckNoiseHearing(FLOAT Loudness);
	int TestCanSeeMe(APlayerPawn *Viewer);

	// Special editor behavior
	AActor* GetHitActor();

	AActor* DetachLimb(class FName, class UClass *);//ADDED_
	AActor* GetProp(int);
	short fixedTurn(short, short, int, int);
	//bool  GetAnimPlacement(class DWI::Placement3 *);
	//bool  PlayAnim(class FName, struct DWI::AnimChannelSpec const &);
	class UMRMActorData* GetMRMActorData(void);
	bool bDoMRM(void)const;
	FName JointName(int)const;
	FVector LightDirection(class FVector const &);
	class UAnimState*  GetAnimState(void)const;
	float GetNotifyTime(class FName, class FName, float);
	float GetScale(void)const;
	float LightContribution(class FVector const &);
	int IsCommonBase(class AActor const *, class AActor const *)const;
	//int JointIndex(class FName, class DWI::Placement3 *)const;
	int JointLocIndex(class FName)const;
	int NumJoints(void)const;
	int PlaySound(class USound *, int, bool, float, bool, float, float, int);
	virtual bool IsDecal(void)const;
	virtual class UParticleList* GetParticleList(void);
	virtual void AdjustScale(void);
	virtual void SetRenderBound(float);
	void AddDynamic(class FName, class FVector const &, class FVector const &, float);
	//void AddTarget(class FName, class DWI::Rotate3 const &, bool, bool);
	void AddTarget(class FName, class FVector const &, bool, bool);
	void ApplyAnim(bool);
	void ApplyMod(class FName, class UClass *);
	void ApplyMods(void);
	void CheckNoiseHearing(float, float);
	void ClearAnimations(void);
	void ClearTarget(class FName);
	void ClearTargets(void);
	void Debug(void)const;
	void DestroyLimb(class FName);
	void LightContributionAndDirection(class FVector const &, class FVector &, float &);
	void PlayImpactSound(int, class UTexture *, unsigned char, class FVector, float, float, float);
	void SerializeAnimState(class FArchive &, int);
	void SetActorData(int);
	void SetBase(class AActor *, int, int, int);
	void SetDefaults(void);
	void StepAnim(float);
	void UpdateSkelPlacement(void);
	void physAttached(float);

	// Natives.
	DECLARE_FUNCTION(execPollSleep)
	DECLARE_FUNCTION(execPollFinishAnim)
	DECLARE_FUNCTION(execPollFinishInterpolation)

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
