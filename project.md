# Project Plan: 2D Co-op Combat Platformer

Game la 2D platformer cho 2 nguoi choi, lay cam hung tu *Ori and the Blind Forest* nhung lam don gian hon. Muc tieu la co ban demo chay duoc: 2 player di chuyen, nhay, danh quai, nhat do, quang do cho nhau, dat block de vuot chuong ngai, chet tao bia mo, nhat lai bia mo va di toi cong dich.

Tai lieu nay viet theo huong de code: module nao can tao, file nao thuoc ai, class nao chiu trach nhiem gi, interface nao can thong nhat truoc.

---

## 1. MVP gameplay

### Bat buoc co

- 2 player cung choi tren mot man hinh.
- Player 1 va Player 2 co input rieng.
- Moi player co:
  - `health`
  - `hunger`
  - `inventory`: coin, food, heart, block
  - huong nhin trai/phai
- Player co the:
  - chay trai/phai
  - nhay
  - chem quai bang hitbox ngan
  - nhat item
  - quang item ra dat
  - dat block ngay o tile ke ben theo huong dang nhin
- Map co:
  - dat/tuong co collision
  - gai gay damage
  - checkpoint
  - cong dich
- Enemy co:
  - patrol enemy
  - shooter enemy
  - boss don gian
- Khi giet enemy, enemy roi item ngau nhien:
  - coin
  - food
  - heart
  - block
- Khi player chet:
  - inventory chuyen vao tombstone
  - player respawn tai checkpoint voi inventory rong
  - bat ky player nao cham tombstone deu nhat lai toan bo item
- Dieu kien thang:
  - ca 2 player con song cung vao vung goal, hoac
  - neu 1 player da chet, player con song nhat tombstone roi vao goal

### De sau neu con thoi gian

- Dash.
- Double jump.
- Flying enemy.
- Moving platform.
- Boss co nhieu pha.
- Level 3.

---

## 2. Structure tree

Khong liet ke `CMakeLists.txt`, `build/` hay file bien dich. Tree nay chi gom file can code va asset can co.

```text
D:/project/
|-- project.md
|
|-- include/
|   |-- Core/
|   |   |-- Game.h
|   |   |-- GameState.h
|   |   |-- AssetManager.h
|   |   |-- AudioManager.h
|   |   |-- InputManager.h
|   |   |-- Camera.h
|   |
|   |-- World/
|   |   |-- Level.h
|   |   |-- LevelLoader.h
|   |   |-- TileMap.h
|   |   |-- Collision.h
|   |   |-- BlockPlacement.h
|   |   |-- Checkpoint.h
|   |   |-- GoalGate.h
|   |   |-- SpikeTrap.h
|   |
|   |-- Entities/
|   |   |-- Entity.h
|   |   |-- Character.h
|   |   |-- Player.h
|   |   |-- Inventory.h
|   |   |-- DroppedItem.h
|   |   |-- Tombstone.h
|   |   |-- Enemy.h
|   |   |-- PatrolEnemy.h
|   |   |-- ShooterEnemy.h
|   |   |-- BossEnemy.h
|   |   |-- Projectile.h
|   |
|   |-- Combat/
|   |   |-- Hitbox.h
|   |   |-- DamageSystem.h
|   |   |-- LootTable.h
|   |
|   |-- Graphics/
|   |   |-- Animation.h
|   |   |-- Animator.h
|   |
|   |-- UI/
|   |   |-- HUD.h
|   |   |-- MenuScreen.h
|   |
|   |-- Utils/
|       |-- Constants.h
|       |-- MathUtils.h
|
|-- src/
|   |-- main.cpp
|   |
|   |-- Core/
|   |   |-- Game.cpp
|   |   |-- AssetManager.cpp
|   |   |-- AudioManager.cpp
|   |   |-- InputManager.cpp
|   |   |-- Camera.cpp
|   |
|   |-- World/
|   |   |-- Level.cpp
|   |   |-- LevelLoader.cpp
|   |   |-- TileMap.cpp
|   |   |-- Collision.cpp
|   |   |-- BlockPlacement.cpp
|   |   |-- Checkpoint.cpp
|   |   |-- GoalGate.cpp
|   |   |-- SpikeTrap.cpp
|   |
|   |-- Entities/
|   |   |-- Entity.cpp
|   |   |-- Character.cpp
|   |   |-- Player.cpp
|   |   |-- Inventory.cpp
|   |   |-- DroppedItem.cpp
|   |   |-- Tombstone.cpp
|   |   |-- Enemy.cpp
|   |   |-- PatrolEnemy.cpp
|   |   |-- ShooterEnemy.cpp
|   |   |-- BossEnemy.cpp
|   |   |-- Projectile.cpp
|   |
|   |-- Combat/
|   |   |-- Hitbox.cpp
|   |   |-- DamageSystem.cpp
|   |   |-- LootTable.cpp
|   |
|   |-- Graphics/
|   |   |-- Animation.cpp
|   |   |-- Animator.cpp
|   |
|   |-- UI/
|       |-- HUD.cpp
|       |-- MenuScreen.cpp
|
|-- assets/
|   |-- textures/
|   |   |-- player_idle.png
|   |   |-- player_run.png
|   |   |-- player_jump.png
|   |   |-- player_attack.png
|   |   |-- enemy_patrol.png
|   |   |-- enemy_shooter.png
|   |   |-- boss.png
|   |   |-- projectile.png
|   |   |-- coin.png
|   |   |-- food.png
|   |   |-- heart.png
|   |   |-- tombstone.png
|   |   |-- placeable_block.png
|   |   |-- tileset_forest.png
|   |   |-- spike.png
|   |   |-- checkpoint.png
|   |   |-- goal_gate.png
|   |   |-- background_forest.png
|   |
|   |-- audio/
|   |   |-- bgm_level.ogg
|   |   |-- jump.wav
|   |   |-- attack.wav
|   |   |-- player_hurt.wav
|   |   |-- enemy_hurt.wav
|   |   |-- item_pickup.wav
|   |   |-- item_throw.wav
|   |   |-- block_place.wav
|   |   |-- tombstone_spawn.wav
|   |   |-- checkpoint.wav
|   |   |-- victory.wav
|   |
|   |-- fonts/
|   |   |-- main_font.ttf
|   |
|   |-- levels/
|       |-- level1.txt
|       |-- level2.txt
```

---

## 3. Quy uoc level file

`LevelLoader` doc file text trong `assets/levels/`.

```text
# = solid tile
. = empty
1 = spawn Player 1
2 = spawn Player 2
^ = spike
C = checkpoint
G = goal gate
P = patrol enemy spawn
R = shooter enemy spawn
B = boss spawn
```

Goi y `level1.txt` nho:

```text
................................
................................
..1.........................G...
..#####..............P.....####.
......#.......^....#####.......
......#....#########...........
..2...#........................
################################
```

Block do player dat ra se duoc luu truc tiep vao `TileMap` luc runtime, khong can ghi lai vao file.

---

## 4. Class responsibilities

### Core

- `main.cpp`: tao `Game game; game.run();`.
- `Game`: so huu window, state, level, 2 player, manager; dieu khien update/render.
- `GameState`: enum `Menu`, `Playing`, `Paused`, `GameOver`, `Victory`.
- `AssetManager`: load texture/font mot lan, tra ve reference.
- `AudioManager`: quan ly `sf::SoundBuffer`, danh sach `sf::Sound` dang phat va `sf::Music`.
- `InputManager`: tao `InputState` rieng cho Player 1 va Player 2.
- `Camera`: theo trung diem 2 player, gioi han trong map, chan 2 player cach qua xa.

### World

- `Level`: so huu map va cac object trong level; chi update/draw va xoa object chet.
- `LevelLoader`: doc level text, tao tile map, spawn point, enemy, checkpoint, goal.
- `TileMap`: luu grid tile, ve tile, check tile solid/empty, set tile khi dat block.
- `Collision`: resolve collision giua `Character`/`Projectile` voi tile.
- `BlockPlacement`: dat block tai tile ke ben theo huong nhin cua player.
- `Checkpoint`: vung luu respawn position.
- `GoalGate`: kiem tra dieu kien thang co-op.
- `SpikeTrap`: gay damage khi player cham vao.

### Entities

- `Entity`: base class co position, velocity, bounds, alive flag, update/draw.
- `Character`: base cho `Player` va `Enemy`, co health, gravity, facing direction.
- `Player`: movement, attack, health, hunger, inventory, death/respawn.
- `Inventory`: luu so luong `coin`, `food`, `heart`, `block`.
- `DroppedItem`: item tren map, co type, quantity, position, velocity.
- `Tombstone`: bia mo chua inventory cua player da chet.
- `Enemy`: base enemy co health, damage, attack cooldown.
- `PatrolEnemy`: di qua lai va quay dau khi gap tuong/mep.
- `ShooterEnemy`: dung yen, ban projectile theo cooldown.
- `BossEnemy`: boss don gian, mau cao, tan cong lap lai.
- `Projectile`: dan bay theo huong, mat khi cham tile/player.

### Combat

- `Hitbox`: rectangle tan cong tam thoi cua player.
- `DamageSystem`: kiem tra hitbox, enemy contact, projectile hit; tru mau va knockback.
- `LootTable`: random item khi enemy chet.

### Graphics/UI

- `Animation`: thong tin spritesheet, frame count, frame time.
- `Animator`: cap nhat frame hien tai va apply texture rect.
- `HUD`: ve health/hunger/coin/block cho 2 player.
- `MenuScreen`: start menu don gian.

---

## 5. Interface toi thieu can thong nhat

### Entity

```cpp
class Entity {
public:
    virtual ~Entity() = default;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual sf::FloatRect getBounds() const = 0;

    bool isAlive() const;
    void kill();
};
```

### InputState

```cpp
struct InputState {
    bool moveLeft = false;
    bool moveRight = false;
    bool jump = false;
    bool attack = false;
    bool dash = false;
    bool placeBlock = false;
    bool throwItem = false;
};
```

`InputManager` can co:

```cpp
void update();
const InputState& getPlayer1Input() const;
const InputState& getPlayer2Input() const;
```

### Player

```cpp
class Player : public Character {
public:
    void update(float dt, const InputState& input);
    void takeDamage(int damage);
    void heal(int amount);
    void restoreHunger(int amount);

    bool isDead() const;
    bool hasCollectedOwnTombstone() const;

    int getFacingDirection() const; // -1 left, 1 right
    Inventory& getInventory();
    const Inventory& getInventory() const;
};
```

### Level

```cpp
class Level {
public:
    bool loadFromFile(const std::string& path);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    TileMap& getTileMap();
    const TileMap& getTileMap() const;

    void addProjectile(std::unique_ptr<Projectile> projectile);
    void addDroppedItem(std::unique_ptr<DroppedItem> item);
    void addTombstone(std::unique_ptr<Tombstone> tombstone);

    std::vector<std::unique_ptr<Enemy>>& getEnemies();
    std::vector<std::unique_ptr<Projectile>>& getProjectiles();
    std::vector<std::unique_ptr<DroppedItem>>& getDroppedItems();
    std::vector<std::unique_ptr<Tombstone>>& getTombstones();
};
```

### BlockPlacement

```cpp
class BlockPlacement {
public:
    static bool tryPlaceBlock(
        Player& player,
        TileMap& map,
        const std::vector<std::unique_ptr<Enemy>>& enemies,
        const std::vector<std::unique_ptr<DroppedItem>>& items,
        const std::vector<std::unique_ptr<Tombstone>>& tombstones
    );
};
```

Quy tac:

- Neu player nhin phai, tile dat block la tile ben phai.
- Neu player nhin trai, tile dat block la tile ben trai.
- Chi dat neu tile empty.
- Khong dat neu trung player, enemy, item, tombstone.
- Dat thanh cong thi tru 1 block trong inventory.

### DamageSystem

```cpp
class DamageSystem {
public:
    static void handlePlayerAttacks(Player& p1, Player& p2, Level& level);
    static void handleEnemyContact(Player& p1, Player& p2, Level& level);
    static void handleProjectiles(Player& p1, Player& p2, Level& level);
};
```

`DamageSystem` khong so huu object, chi nhan tham chieu va thay doi trang thai.

---

## 6. Ownership va memory

Dung `std::unique_ptr` cho object co nhieu class con:

```cpp
std::unique_ptr<Player> player1;
std::unique_ptr<Player> player2;
std::vector<std::unique_ptr<Enemy>> enemies;
std::vector<std::unique_ptr<Projectile>> projectiles;
std::vector<std::unique_ptr<DroppedItem>> droppedItems;
std::vector<std::unique_ptr<Tombstone>> tombstones;
```

Ly do:

- `Enemy` co nhieu class con.
- Xoa object qua pointer lop cha can destructor dung.
- Giam loi memory leak.

Bat buoc:

- `Entity` co `virtual ~Entity() = default;`.
- `Enemy` co `virtual ~Enemy() = default;`.
- Khong dung raw `new/delete` trong gameplay code.

---

## 7. Input cho 2 player

| Hanh dong | Player 1 | Player 2 |
| :--- | :--- | :--- |
| Di trai | A | Left |
| Di phai | D | Right |
| Nhay | W | Up |
| Tan cong | F | J |
| Dash | G | K |
| Dat block | H | L |
| Quang do | R | O |

Dung phim chu cho Player 2 de chay duoc tren laptop khong co numpad.

---

## 8. Constants can co

Dat trong `include/Utils/Constants.h`.

```cpp
constexpr int TILE_SIZE = 32;

constexpr float GRAVITY = 1200.0f;
constexpr float PLAYER_SPEED = 220.0f;
constexpr float PLAYER_JUMP_SPEED = -480.0f;
constexpr float MAX_PLAYER_DISTANCE = 900.0f;

constexpr int PLAYER_MAX_HEALTH = 5;
constexpr int PLAYER_MAX_HUNGER = 100;
constexpr float HUNGER_LOSS_PER_SECOND = 1.0f;
constexpr float HUNGER_DAMAGE_INTERVAL = 1.0f;

constexpr int PATROL_ENEMY_HEALTH = 2;
constexpr int SHOOTER_ENEMY_HEALTH = 3;
constexpr int BOSS_HEALTH = 20;

constexpr int START_BLOCK_COUNT = 3;
```

---

## 9. Chia viec cho 2 nguoi

### Dev A: Core + World

Dev A so huu:

```text
Core/Game
Core/GameState
Core/AssetManager
Core/AudioManager
Core/Camera
World/Level
World/LevelLoader
World/TileMap
World/Collision
World/BlockPlacement
World/Checkpoint
World/GoalGate
World/SpikeTrap
Utils/Constants
Utils/MathUtils
main.cpp
assets/levels/*
assets/textures/tileset_forest.png
assets/textures/background_forest.png
assets/textures/placeable_block.png
assets/textures/spike.png
assets/textures/checkpoint.png
assets/textures/goal_gate.png
```

Dev A khong viet AI enemy, combat, inventory, HUD.

### Dev B: Gameplay objects + UI

Dev B so huu:

```text
Core/InputManager
Entities/Entity
Entities/Character
Entities/Player
Entities/Inventory
Entities/DroppedItem
Entities/Tombstone
Entities/Enemy
Entities/PatrolEnemy
Entities/ShooterEnemy
Entities/BossEnemy
Entities/Projectile
Combat/Hitbox
Combat/DamageSystem
Combat/LootTable
Graphics/Animation
Graphics/Animator
UI/HUD
UI/MenuScreen
assets/textures/player_*.png
assets/textures/enemy_*.png
assets/textures/boss.png
assets/textures/projectile.png
assets/textures/coin.png
assets/textures/food.png
assets/textures/heart.png
assets/textures/tombstone.png
assets/audio/*
assets/fonts/main_font.ttf
```

Dev B khong sua `TileMap`, `Collision`, `LevelLoader` neu chua thong nhat voi Dev A.

---

## 10. Giai doan code song song

### Giai doan 1: Skeleton compile duoc

Dev A:

- Tao `Game`, `GameState`, `AssetManager`, `AudioManager`, `Camera`.
- Tao `Level`, `LevelLoader`, `TileMap`, `Collision` skeleton.
- Tao `Constants.h`.
- Tao `main.cpp`.

Dev B:

- Tao `InputManager` va `InputState`.
- Tao `Entity`, `Character`, `Player` skeleton.
- Tao `HUD`, `MenuScreen` skeleton.

Diem ghep:

- `Game` tao 2 `Player`.
- `Game` goi `InputManager.update()`.
- `Game` goi `Player.update(dt, input)`.

Ket qua:

- Chuong trinh compile.
- Mo cua so.
- Ve 2 hinh chu nhat dai dien player.

### Giai doan 2: Movement + map collision

Dev A:

- Hoan thien `TileMap` doc `level1.txt`.
- Hoan thien collision tile.
- Hoan thien camera theo 2 player.

Dev B:

- Hoan thien player move, jump, gravity.
- Them facing direction.
- Them health/hunger field.

Diem ghep:

- Sau khi player update velocity, goi `Collision::resolveTileCollision`.

Ket qua:

- 2 player chay nhay tren map va khong xuyen dat.

### Giai doan 3: Combat + enemy co ban

Dev A:

- `LevelLoader` spawn `PatrolEnemy` tu ky tu `P`.
- `Level` update/draw enemy list.
- Them `SpikeTrap`.

Dev B:

- Viet `Enemy`, `PatrolEnemy`.
- Viet `Hitbox`, `DamageSystem`.
- Player chem duoc enemy.
- Enemy cham player gay damage.

Ket qua:

- Co quai tuan tra.
- Player chem quai duoc.
- Player mat mau khi cham quai/gai.

### Giai doan 4: Item + inventory + tombstone

Dev A:

- `Level` quan ly `DroppedItem` va `Tombstone`.
- Them checkpoint respawn.
- Them `GoalGate`.

Dev B:

- Viet `Inventory`.
- Viet `DroppedItem`.
- Viet `LootTable`.
- Viet `Tombstone`.
- Player nhat coin/food/heart/block.
- Player chet tao tombstone va respawn voi inventory rong.

Ket qua:

- Giet quai roi item.
- Nhat item duoc.
- Chet tao bia mo.
- Nhat lai bia mo duoc.

### Giai doan 5: Block placement + throw item

Dev A:

- Hoan thien `BlockPlacement`.
- `TileMap` cho set tile thanh placeable block.
- Kiem tra vi tri dat block hop le.

Dev B:

- Them input dat block va quang do.
- Player goi `BlockPlacement::tryPlaceBlock`.
- Player quang item ra truoc mat thanh `DroppedItem`.
- HUD hien coin, block, health, hunger cua ca 2 player.

Ket qua:

- Player dat block ngay o tile ke ben theo huong nhin.
- Player quang do cho nguoi kia nhat.

### Giai doan 6: Shooter + boss + polish

Dev A:

- `LevelLoader` spawn `ShooterEnemy` va `BossEnemy`.
- Hoan thien state `Victory`, `GameOver`, `restartLevel`.
- Can bang level.

Dev B:

- Viet `ShooterEnemy`, `Projectile`, `BossEnemy`.
- Them animation co ban.
- Them audio.
- Hoan thien menu, HUD, victory/game over screen.

Ket qua:

- Co level choi duoc tu dau toi cuoi.
- Co boss.
- Co dieu kien thang/thua ro rang.

---

## 11. Quy tac chet, respawn va tombstone

- Khi player health ve 0:
  - tao `Tombstone` tai vi tri an toan gan noi chet
  - chuyen toan bo inventory vao tombstone
  - clear inventory cua player
  - player respawn sau 2 giay tai checkpoint
  - player respawn voi health day va hunger = 50
- Neu player roi xuong vuc:
  - tombstone tao tai checkpoint gan nhat hoac nen dat gan mep vuc truoc do
- Bat ky player nao cham tombstone deu nhat toan bo item trong do.
- Neu ca hai player cung chet:
  - restart tu checkpoint gan nhat
  - neu muon kho hon thi chuyen `GameOver`

---

## 12. Quy tac loot va item

`LootTable` random khi enemy chet:

```text
50% coin
25% food
15% heart
10% block
```

Tac dung item:

- `coin`: cong diem/tien.
- `food`: hoi hunger.
- `heart`: hoi health.
- `block`: cong so block co the dat.

Quang item:

- Moi lan bam `throwItem`, quang 1 item dang chon.
- Ban demo co the mac dinh uu tien quang block, neu khong co block thi food, heart, coin.
- Item duoc tao phia truoc player theo huong nhin.

---

## 13. Quy tac code chung

- Header dung `#pragma once`.
- Class dung `PascalCase`.
- Ham va bien dung `camelCase`.
- Khong dung `using namespace std;` trong header.
- Constant gameplay dat trong `Constants.h`.
- Khong de `Level` xu ly logic combat/inventory.
- `DamageSystem`, `Collision`, `BlockPlacement` chi nhan tham chieu, khong so huu object.
- Moi commit chi nen gom mot tinh nang nho.
- Khong sua file cua nguoi kia neu chua thong nhat.

---

## 14. Pham vi de giu du an de code

De demo chac chan hoan thanh, tam thoi khong lam:

- Save/load file.
- Multiplayer online.
- Nhieu hon 2 player.
- Inventory co slot phuc tap.
- Cay ky nang.
- Pha block.
- Moving platform trong giai doan dau.
- Animation qua chi tiet.
