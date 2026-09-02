# Appendix: AI Usage Notes

> This declaration was prepared according to the supplied *AI Usage Guidelines*. Before submission, the student must replace every bracketed field, verify the access times against the exported chat history, and confirm that every statement accurately describes their own work.

## 1. Student and project information

- **Student name:** Huỳnh Trần Gia Hân
- **Student ID:** 25125042
- **Class:** 25A02
- **Project:** CS202 Final Project
- **Repository:** `CS202-FinalProject_SuperMario_Rubric`
- **Team role:** Dev B — Gameplay Objects and UI

## 2. Declaration

I declare that I used an AI coding assistant during this project. The AI was used as a support tool for inspecting the existing repository, explaining code, identifying defects, suggesting or applying scoped changes, checking builds, and helping organize project documentation.

I did not treat AI output as an authoritative source. The project requirements, assets, gameplay decisions, existing source code, and final acceptance decisions were supplied or controlled by the team. AI-assisted changes were reviewed against the repository interfaces and were build-checked. Any remaining manual gameplay checks are identified below and must be completed or confirmed by the student before submission.

## 3. Tool information

| Required detail | Declaration |
| --- | --- |
| Tool | OpenAI Codex & Google Gemini 3 |
| Model/version | GPT-5-based Codex coding agent & Gemini 3 |
| Platform | Codex coding workspace / desktop application & Antigravity IDE |
| Access period | June - September 2026, approximately 17:00–22:40 ICT (UTC+7), based on repository activity and the current session. |
| Main purposes | Repository inspection, C++/SFML debugging, scoped implementation, merge-conflict recovery, build verification, and documentation assistance |
| Evidence | Link to Codex conversations and Link to Antigravity conversations |

## 4. Prompt and usage log

The prompts below preserve the substantive wording and technical requirements used during the project. Repeated coding constraints and very long file lists are shortened where indicated. The complete wording should be retained through the attached chat export or screenshots.

### Prompt 1 — Repository context and ownership

- **Prompt used:**

> You are an expert C++ game developer assisting me with building a 2D Co-op Combat Platformer using SFML. Before writing or suggesting any code, read the project plan, inspect the real repository structure, respect Dev A/Dev B ownership, preserve shared interfaces, and follow the specified modern C++ conventions. Acknowledge the constraints and summarize the current codebase and next step for Dev B.

- **Purpose:** Establish repository context, architecture rules, code ownership, and coding standards.
- **AI-assisted content:** Repository survey and architectural summary.
- **Student/team contribution and validation:** The team supplied the plan, ownership boundaries, architecture, and standards.

### Prompt 2 — Mushroom enemy and projectile asset migration

- **Prompt used:**

> I reorganized the enemy and projectile assets into dedicated folders. Update all references: ShooterEnemy should use the new mushroom spritesheets, mushroom projectile should use `assets/textures/mushroom/mushroom_projectile.png`, and the boss projectile should use its boss asset. Update `Game.cpp`, `ShooterEnemy`, and `Projectile`, preserve interfaces, and keep the required C++ header/style rules.

- **Purpose:** Update texture registrations and sprite handling after asset reorganization.
- **AI-assisted content:** Asset-reference analysis and scoped C++ patches.
- **Student/team contribution and validation:** The student supplied and organized the new artwork and specified the correct paths and animation intent.

### Prompt 3 — Stationary ShooterEnemy and pixel-art scaling

- **Prompt used:**

> Update `ShooterEnemy` so it remains stationary, receives no knockback or displacement, keeps horizontal velocity at zero, uses 32×32 frames scaled by 2.0 to 64×64, keeps texture smoothing disabled, and has collision bounds matching the 64×64 render size. Modify only Dev B files.

- **Purpose:** Correct mushroom movement, collision size, and pixel-art presentation.
- **AI-assisted content:** Review and implementation guidance for rooted movement, damage reaction, animation rectangles, and bounds.
- **Student/team contribution and validation:** The student defined the intended enemy behavior and verified the correct source-frame size.

### Prompt 4 — Mouse menus and key rebinding

- **Prompt used:**

> Replace keyboard menu selection with mouse hover and left-click interaction. Implement action-based key rebinding using an `Action` enum and `std::unordered_map`, prevent duplicate bindings, display key names, and support the click-an-action then press-a-key workflow with Escape to cancel. Preserve Dev A interfaces and use `mapPixelToCoords`.

- **Purpose:** Introduce mouse-driven UI interaction and configurable controls.
- **AI-assisted content:** UI/input architecture review, event-flow suggestions, and code changes.
- **Student/team contribution and validation:** The student selected the desired workflow and control actions.

### Prompt 5 — PatrolEnemy audit

- **Prompt used:**

> Review and audit `PatrolEnemy` against the specification: constant horizontal patrol, gravity, wall turnaround, forward ledge detection, 25–30 HP, horizontal sword knockback without changing facing direction, hurt/death states, and loot drops. Report flaws and provide an exact patch without breaking `TileMap` or `Level`.

- **Purpose:** Validate slime movement, collision, combat, death, and loot logic.
- **AI-assisted content:** Code audit findings and proposed patch.
- **Student/team contribution and validation:** The student provided the target gameplay specification and ownership constraints.

### Prompt 6 — FlyingEnemy and BossEnemy audit

- **Prompt used:**

> Audit `FlyingEnemy`, `BossEnemy`, `DamageSystem.cpp`, and `Constants.h`. FlyingEnemy must ignore gravity, move smoothly, reverse at walls, receive knockback, and drop loot. BossEnemy must be knockback-immune, use a two-phase FSM, track the nearest living player, flip correctly, and use boss constants and the existing projectile asset key.

- **Purpose:** Identify runtime, physics, combat, asset, and FSM inconsistencies.
- **AI-assisted content:** Evidence-based code audit and recommended changes.
- **Student/team contribution and validation:** The student supplied the intended AI/FSM behavior and asset constraints.

### Prompt 7 — FlyingEnemy fixes and BossEnemy multi-phase FSM

- **Prompt used:**

> Apply the audit fixes in `Constants.h`, `FlyingEnemy.cpp`, `BossEnemy.h`, and `BossEnemy.cpp`. Add the specified flying/boss constants, fix FlyingEnemy origin and collision/knockback integration, fix boss wall jitter, enforce knockback immunity, and implement PhaseOne/Enraged attacks with spread projectiles and shockwaves. Do not change existing projectile texture registrations.

- **Purpose:** Resolve flying-enemy runtime defects and implement the boss phase FSM.
- **AI-assisted content:** Scoped C++ implementation and build verification.
- **Student/team contribution and validation:** The student chose constants, thresholds, attacks, and exact target files.

### Prompt 8 — Hunger and reclaiming placed blocks

- **Prompt used:**

> Implement hunger depletion at 0.45 per second, starvation damage at zero hunger, and +20 hunger from food. Add a mechanic that lets a player attack/interact with the tile directly in front, reclaim only player-placed blocks, reset the tile to air, and return one block to inventory while leaving natural geometry untouched.

- **Purpose:** Add survival pacing and safe block-recovery gameplay.
- **AI-assisted content:** Hunger-loop and placed-block query/reclaim implementation.
- **Student/team contribution and validation:** The student supplied the formulas, interaction rule, and natural-tile safety requirement.

### Prompt 9 — Audio integration

- **Prompt used:**

> Integrate the audio assets under `assets/audio/` using the existing `AudioManager`. Cache sound buffers, support concurrent sounds, loop gameplay music, play character-specific attack audio, and trigger sounds for jumping, placing blocks, eating, dropping, pickups, menu clicks, game over, and victory. Missing files must warn rather than crash.

- **Purpose:** Connect sound effects and music to gameplay/UI state transitions.
- **AI-assisted content:** Audio-manager review, event-hook integration, and build checking.
- **Student/team contribution and validation:** The student supplied all audio assets, names, and desired trigger events.

### Prompt 10 — Merge-conflict assistance

- **Prompt used:**

> Hey, help me fix this conflict.

> [Two attached/pasted versions of the application loop and state-transition code were provided.]

- **Purpose:** Reconcile conflicting versions without losing gameplay or UI behavior.
- **AI-assisted content:** Comparison of the conflicting code and a merged resolution.
- **Student/team contribution and validation:** The student provided both conflict sides and retained responsibility for accepting the merge.

### Prompt 11 — Missing left-click selection logic

- **Prompt used:**

> Check giùm tui sao cái logic click chọn bằng chuột trái bị mất rồi vậy?

> Okay, can you help me fix that? It was probably lost because the conflict resolution was not good, so our code logic was not applied here.

- **Purpose:** Diagnose and restore mouse hover/click behavior lost during conflict resolution.
- **AI-assisted content:** Regression tracing and restoration of click-selection logic.
- **Student/team contribution and validation:** The student identified the visible regression and connected it to the merge conflict.

### Prompt 12 — Report based on the real repository

- **Prompt used:**

> Read our project once again — read the real project, not only `project.md` — and use the supplied report structure to write the report in `Report.md`.

- **Purpose:** Structure and draft project documentation using evidence from the implemented repository.
- **AI-assisted content:** Repository survey, report organization, architecture explanation, diagrams, implementation summary, and known-issues draft.
- **Student/team contribution and validation:** The implementation itself, gameplay decisions, assets, Git history, and project evidence came from the team. **The student must personally review, rewrite where necessary, and be able to explain every report section before submission.**

### Prompt 13 — Comparison with the normal project requirements

- **Prompt used:**

> Read `CS202-FinalProject_SuperMario (1).docx` for the normal group project, check which required features our project also has, identify additional features, assess whether our scope is twice the original requirements, and update `Report.md`.

- **Purpose:** Compare the real implementation with the official baseline and avoid unsupported claims.
- **AI-assisted content:** Requirement matrix, added-feature inventory, design-pattern evidence, and a qualified “approximately 2× gameplay scope” assessment.
- **Student/team contribution and validation:** The student supplied the official document and requested a conservative comparison. The report explicitly records missing bonuses and incomplete runtime paths.

### Prompt 14 — PNG selectors for the Player 1/Player 2 keybind tabs

- **Prompt used:**

> Check where the key-binding setting is. Redraw the “Player 1” and “Player 2” selectors using `selector1.png` and `selector2.png` instead of SFML rectangle shapes.

- **Purpose:** Replace temporary UI primitives with the final selector artwork without breaking mouse interaction.
- **AI-assisted content:** Located `MenuScreen::drawControls`, replaced the tab rectangles with texture sprites, corrected swapped texture registrations, preserved shared hitboxes, and build-checked the project.
- **Student/team contribution and validation:** The student supplied the selector artwork and visual direction. Manual visual confirmation in the running game remains the student's responsibility.

## 5. Summary of AI-generated or AI-assisted content

AI assistance was used for:

- Repository inspection and explanation of existing architecture.
- Audits of enemy movement, collision, knockback, death, loot, and boss-state logic.
- Suggested or applied changes to scoped C++/SFML gameplay and UI files.
- Recovery of mouse interaction after a merge conflict.
- Build and diff checks after code changes.
- Initial drafting and restructuring of parts of `Report.md`.
- Requirement comparison against the supplied normal-project DOCX.
- Initial drafting of AI Usage Notes appendix.

AI was **not** the source of the project's original assignment, team ownership model, artwork, audio assets, desired game rules, or final product decisions. Those were supplied by the student/team or already present in the repository.

## 6. Student editing, understanding, and validation

Before submitting this declaration and the project, I confirm that I have:

- [ ] Read every AI-assisted source-code change.
- [ ] Built the final project successfully.
- [ ] Manually tested mouse navigation and Player 1/Player 2 key rebinding.
- [ ] Tested player movement, attacks, hunger, block placement/reclaim, enemy damage, boss phases, audio, victory, and game-over flows.
- [ ] Verified that asset paths match the files committed to the repository.
- [ ] Reviewed and revised `Report.md` in my own words where necessary.
- [ ] Verified every factual statement in the report against the actual source code.
- [ ] Confirmed that I can explain all submitted code and report content during an oral examination.

## 7. Supporting evidence

**Evidence location/link:** https://chatgpt.com/s/cx_6a98452b83bc819183f223e6ed5c6891

## 8. Final acknowledgement

I acknowledge that I am responsible for the accuracy, integrity, and explainability of the submitted project. I understand that undeclared AI-generated code or report content, blind copying, fabricated evidence, or an inability to explain the submitted work may constitute an academic-integrity violation.
