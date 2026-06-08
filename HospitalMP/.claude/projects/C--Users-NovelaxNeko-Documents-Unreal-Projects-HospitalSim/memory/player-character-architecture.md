---
name: player-character-architecture
description: How the player pawn, spawn, and modular AssetsvilleHospital character are set up
metadata:
  type: project
---

The spawned player pawn is `/Game/Blueprints/BP_BaseCharacter` (set as `DefaultPawnClass` on `/Game/Blueprints/BP_BaseMode`; controller `BP_BaseController`, GameState `BP_ClinicalGameState`, HUD `HUD_InGame`). BP_BaseCharacter holds the gameplay wiring: enhanced input (IA_MoveForward/MoveRight/Look), headbob camera shake, name-tag widget, and save/role loading (`BP_GameInstance` → `SG_SaveGame` → `LoadPlayerData`, `E_ClinicalRole`, `BP_ClinicalPlayerState`).

The character is a **leader-pose modular character** using AssetsvilleHospital parts in `/Game/AssetsvilleHospital/Meshes/Characters/` (heads, hair, faces, tops like SK_top_doctorScrub/Scrub/suit, pants, shoes, hands, addons — male + female variants). ALL parts are skinned to `UE4_Mannequin_Skeleton` (the only humanoid skeleton in that pack), so the animated base mesh (`SK_Mannequin`) drives them via `Set Leader Pose Component`. The correct AnimBP for this skeleton is `ThirdPerson_AnimBP` (in TPP_Template/Mannequin/Animations). The reference rig with all 12 part slots (Head/Hair/Face/FacialHair/Headwear/Hands/Top/Pants/Shoes/Addon1-3) lives on `/Game/.../TPP_Template/ThirdPersonBP/Blueprints/ThirdPersonCharacter`.

Gotcha: `ABP_Character` (`/Game/Animations/ABP_Character`) is built for `T-Pose_Skeleton` (the Mixamo MainCharacter in `/Game/Character/MainCharacter/T-Pose`), NOT the mannequin skeleton — assigning it to a SK_Mannequin-based mesh makes the character T-pose/static (looks like a default "unreal mannequin"). Use ThirdPerson_AnimBP for mannequin-skeleton characters. See [[neostack-inherited-component-limitation]].
