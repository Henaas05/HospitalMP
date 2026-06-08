---
name: neostack-inherited-component-limitation
description: NeoStack Lua API cannot edit inherited component defaults; force-compile blocked by Live Coding
metadata:
  type: reference
---

Two NeoStack `execute_script` constraints hit while editing Character Blueprints:

1. **Inherited components are not addressable** via `a:get/set/set_property/list_properties`. On a Character BP, the inherited base mesh ("Mesh" / "CharacterMesh0") returns "target not found" for every setter. Only SCS-added components (e.g. ones you add with `add_component`) and graph nodes are editable. Workaround: do it in a graph instead — e.g. fix the base mesh's anim at runtime with a `Set Anim Instance Class` node in the Construction Script (`Get Mesh` → target), and use `Set Leader Pose Component` nodes for modular parts. New components added via `add_component(name, class, parent)` (parent can be the inherited "CharacterMesh0") ARE addressable for `a:set(name, "SkeletalMeshAsset", "/Game/...")` after `a:refresh()`.

2. **Compile can be blocked**: `a:compile()` fails with "Live Coding patches active and instances exist. Use force=true to override." No compile() argument form (`true`, `{force=true}`, `{Force=true}`) actually overrides it, and engine `CompileBlueprint` isn't reachable via `invoke`. Resolution is to stop PIE and restart the editor. `a:save()` still persists the source structure, so edits aren't lost — they compile on next editor load.

**Why:** these shape how character/pawn edits must be done in this project. **How to apply:** for base-mesh/anim/visibility changes use Construction-Script nodes, not component-default setters; after structural edits, expect a possible compile block and tell the user to restart the editor rather than assuming the edit failed.
