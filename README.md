# CardDemo开发文档

## 功能演示
以下是游戏功能演示视频：

<div align="center">
  <video width="640" height="480" controls autoplay>
    <source src="media/demo.mp4" type="video/mp4">
    你的浏览器不支持视频播放功能，请下载视频查看。
  </video>
</div>


### LevelConfig 开发文档

#### 1. 类概述
`LevelConfig` 类用于存储和管理游戏关卡中扑克牌的初始布局信息，支持从JSON格式配置文件加载数据。


#### 2. 核心结构
- **CardInfo 结构体**：
  - `cardFace`：牌面点数(1-13)
  - `cardSuit`：牌面花色(0-3)
  - `position`：卡牌在游戏场景中的位置坐标


#### 3. 主要成员变量
- `playfieldCards`：游戏区域扑克牌布局信息数组
- `stackCards`：牌堆区域扑克牌布局信息数组


#### 4. 关键方法
- `parseFromJson(jsonStr)`：从JSON字符串解析关卡配置
  - **参数**：JSON格式的关卡配置字符串
  - **返回值**：解析成功返回true，失败返回false


#### 5. 使用示例
```cpp
// 加载关卡配置
LevelConfig levelConfig;
bool success = levelConfig.parseFromJson(jsonData);

if (success) {
    // 使用 levelConfig.playfieldCards 和 levelConfig.stackCards
    // 生成游戏场景中的扑克牌布局
}
```


#### 6. JSON 格式示例
```json
{
  "playfieldCards": [
    { "cardFace": 1, "cardSuit": 2, "position": { "x": 100, "y": 200 } },
    { "cardFace": 2, "cardSuit": 3, "position": { "x": 200, "y": 200 } }
  ],
  "stackCards": [
    { "cardFace": 3, "cardSuit": 0, "position": { "x": 500, "y": 100 } }
  ]
}
```




### LevelConfigLoader 开发文档

#### 1. 类概述
`LevelConfigLoader` 类提供静态方法，用于从文件或Cocos2d-x资源系统加载关卡配置数据到 `LevelConfig` 对象。


#### 2. 核心方法
- **loadLevelConfig(filePath, config)**
  - **功能**：从指定文件路径加载JSON格式的关卡配置
  - **参数**：
    - `filePath`：配置文件路径
    - `config`：用于存储加载结果的LevelConfig对象引用
  - **返回值**：加载成功返回true，失败返回false

- **loadLevelConfigFromResource(levelId, config)**
  - **功能**：从Cocos2d-x资源系统加载指定关卡ID的配置
  - **参数**：
    - `levelId`：关卡ID（如1表示第一关）
    - `config`：用于存储加载结果的LevelConfig对象引用
  - **返回值**：加载成功返回true，失败返回false


#### 3. 使用示例
```cpp
// 从文件加载
LevelConfig config;
bool success = LevelConfigLoader::loadLevelConfig("levels/level1.json", config);

// 从资源加载
LevelConfigLoader::loadLevelConfigFromResource(2, config); // 加载第2关配置
```


### GameController 开发文档  

#### 1. 类概述  
`GameController` 是游戏的核心控制器，负责协调游戏模型（`GameModel`）、视图（`GameView`）和关卡逻辑，处理卡牌交互、撤销操作及关卡加载等核心流程。  


#### 2. 核心职责  
- **流程控制**：管理游戏初始化、关卡加载和卡牌生成。  
- **交互处理**：响应卡牌点击事件，处理储备区/游戏区域的逻辑判断。  
- **状态同步**：维护卡牌控制器集合，确保模型与视图状态一致。  
- **撤销管理**：通过`UndoManager`记录和执行撤销操作。  


#### 3. 关键方法  
| 方法名                     | 功能描述                                                                 |  
|----------------------------|--------------------------------------------------------------------------|  
| `loadLevel(levelId)`       | 加载指定关卡配置，初始化游戏状态。                                       |  
| `handleCardClick(cardId)`   | 统一处理卡牌点击事件，区分储备区和游戏区域逻辑。                           |  
| `recordUndoAction(...)`    | 记录撤销操作，用于回退卡牌移动等状态变更。                                 |  
| `generateCardsFromModel()`  | 根据`GameModel`数据生成卡牌视图和控制器，绑定模型与视图。                   |  


#### 4. 组件关联  
- **Model**：通过`GameModel`管理游戏状态（如卡牌布局、匹配状态）。  
- **View**：通过`GameView`渲染游戏界面，接收用户输入。  
- **Controllers**：通过`CardController`集合管理单张卡牌的交互逻辑。  
- **Managers**：通过`UndoManager`实现撤销功能。  


#### 5. 使用示例  
```cpp
// 创建游戏主控制器
auto gameController = GameController::createInstance();
gameController->loadLevel(1); // 加载第一关

// 处理卡牌点击事件
void GameController::handleCardClick(int cardId) {
    auto card = findCardById(cardId);
    if (isCardInReserve(card)) {
        handleReserveCardClick(card);
    } else if (isCardInPlayField(card)) {
        handlePlayFieldCardClick(card);
    }
}
```  



### UndoManager 开发文档  

#### 1. 类概述  
`UndoManager` 用于管理游戏中的可撤销操作，通过操作栈记录历史变更，支持撤销最后一次操作并清空记录。  


#### 2. 核心功能  
- **操作记录**：通过`recordAction`保存撤销操作到栈中。  
- **撤销判断**：通过`canUndo`检查是否存在可撤销的操作。  
- **唯一ID生成**：通过`getNextUndoId`生成递增的操作ID。  
- **撤销执行**：通过`undoLastAction`回退最后一次操作到游戏模型。  


#### 3. 关键方法  
| 方法名               | 功能描述                                                                 |  
|----------------------|--------------------------------------------------------------------------|  
| `recordAction(action)` | 向撤销栈中添加一个操作记录（如卡牌移动、状态变更）。                     |  
| `canUndo()`          | 判断撤销栈是否非空，即是否有操作可以撤销。                               |  
| `undoLastAction(model)` | 从栈顶取出最后一个操作，执行撤销逻辑（需依赖GameModel实现具体回退）。    |  
| `clearHistory()`     | 清空所有已记录的撤销操作，用于重置游戏状态。                             |  


#### 4. 使用示例  
```cpp
// 记录一个卡牌移动操作
UndoAction moveAction;
moveAction.type = UndoActionType::MOVE_CARD;
moveAction.cardId = 5;
moveAction.oldPosition = Vec2(100, 200);
undoManager.recordAction(moveAction);

// 撤销最后一次操作
if (undoManager.canUndo()) {
    undoManager.undoLastAction(gameModel);
}

// 清空撤销历史
undoManager.clearHistory();
```



### CardModel 开发文档

#### 1. 类概述
`CardModel` 是扑克牌的数据模型，用于存储和管理单张扑克牌的属性和状态，包括花色、点数、位置及游戏状态（是否翻开、匹配等）。


#### 2. 核心属性
| 属性名           | 类型         | 描述                         |
|------------------|--------------|------------------------------|
| `_cardId`        | int          | 卡牌唯一标识符               |
| `_suit`          | CardSuit     | 卡牌花色（梅花、方块等）     |
| `_face`          | CardFace     | 卡牌点数（A、2-10、JQK）     |
| `_isFlipped`     | bool         | 卡牌是否正面朝上             |
| `_isMatched`     | bool         | 卡牌是否已匹配               |
| `_position`      | Vec2         | 卡牌在场景中的位置           |
| `_isActive`      | bool         | 卡牌是否可交互               |
| `_originalZOrder`| int          | 卡牌原始渲染层级             |


#### 3. 关键方法
| 方法名           | 功能描述                                                                 |
|------------------|--------------------------------------------------------------------------|
| `flipCard()`     | 翻转卡牌状态（翻开/关闭）。                                              |
| `canMatchWith()` | 判断两张牌是否可以匹配（点数相差1）。                                    |
| `setMatched()`   | 设置卡牌匹配状态。                                                        |
| `setActive()`    | 设置卡牌是否可交互（如禁用已匹配的卡牌）。                               |


#### 4. 使用示例
```cpp
// 创建一张红桃A
CardModel card(CardSuit::SUIT_HEARTS, CardFace::FACE_A);
card.setId(1);
card.setPosition(Vec2(100, 200));

// 翻开卡牌
card.flipCard();

// 检查与另一张牌是否匹配
bool canMatch = card.canMatchWith(otherCard);

// 标记为已匹配
if (canMatch) {
    card.setMatched(true);
    otherCard->setMatched(true);
}
```


#### 5. 匹配规则
两张牌可以匹配的条件：
- 两张牌均未匹配
- 两张牌点数相差为1（如A和2、10和J）



### GameModel 开发文档  

#### 1. 类概述  
`GameModel` 是游戏的核心数据模型，负责管理扑克牌在游戏区域（Play Field）、牌堆（Stack）和储备区（Reserve）的分组及状态，提供关卡初始化、卡牌移动和数据查询功能。  


#### 2. 核心属性  
| 属性名               | 类型                          | 描述                         |  
|----------------------|-------------------------------|------------------------------|  
| `_playFieldCards`    | vector<CardModel*>            | 游戏区域可交互卡牌列表       |  
| `_stackCards`        | vector<CardModel*>            | 牌堆区域卡牌列表（后进先出） |  
| `_reserveCards`      | vector<CardModel*>            | 储备区初始布局卡牌列表       |  
| `_currentStackTop`   | CardModel*                    | 当前牌堆顶部卡牌（缓存值）   |  


#### 3. 关键方法  
| 方法名                     | 功能描述                                                                 |  
|----------------------------|--------------------------------------------------------------------------|  
| `initWithLevelConfig(config)` | 根据关卡配置初始化各区域卡牌，解析`LevelConfig`中的`CardInfo`数据。      |  
| `moveTopCardToStack()`     | 将储备区顶部卡牌移动到牌堆，通常用于回合开始时补充牌堆。                 |  
| `replaceTopStackCard(card)` | 用指定卡牌替换牌堆顶部卡牌，用于实现卡牌移动或匹配逻辑。                 |  
| `undoReplaceTopStackCard(...)` | 撤销卡牌替换操作，恢复原卡牌位置及状态（配合撤销管理器使用）。           |  


#### 4. 区域划分  
- **游戏区域（PLAY_FIELD）**：玩家可直接交互的卡牌区域，支持点击和移动。  
- **牌堆（STACK）**：用于临时存放卡牌的堆叠区域，仅顶部卡牌可操作。  
- **储备区（RESERVE）**：关卡初始布局的卡牌区域，通常作为卡牌来源。  


#### 5. 使用示例  
```cpp
// 初始化游戏模型
GameModel model;
LevelConfig config;
if (model.initWithLevelConfig(config)) {
    // 获取游戏区域卡牌数量
    int playFieldCount = model.getPlayFieldCards().size();
    
    // 移动储备区顶部卡牌到牌堆
    model.moveTopCardToStack();
    
    // 替换牌堆顶部卡牌（例如匹配成功后）
    CardModel* matchedCard = ...;
    model.replaceTopStackCard(matchedCard);
}
```  




### UndoAction 开发文档  

#### 1. 结构体概述  
`UndoAction` 用于记录可撤销操作的详细信息，配合 `UndoManager` 实现游戏状态回退。每个结构体实例对应一个具体的可撤销操作（如卡牌移动、父节点变更等）。


#### 2. 核心字段  
| 字段名           | 类型               | 描述                                                                 |  
|------------------|--------------------|----------------------------------------------------------------------|  
| `type`           | UndoActionType     | 操作类型（移动、父节点变更、列表变更）。                             |  
| `card`           | CardModel*         | 操作涉及的卡牌模型（必填）。                                         |  
| `originalParent` | Node*              | 卡牌在操作前的父节点（用于恢复视图层级）。                           |  
| `originalPosition` | Vec2        | 卡牌在操作前的位置坐标（用于撤销移动操作）。                         |  
| `field`          | CardField          | 卡牌在操作前所属的游戏区域（游戏区、牌堆、储备区）。                 |  
| `id`             | int                | 操作唯一ID（与撤销栈中的记录对应）。                                 |  


#### 3. 使用场景  
- **卡牌移动**：记录移动前的位置和所属区域，撤销时恢复位置。  
- **层级变更**：记录卡牌的原始父节点（如从牌堆移动到游戏区），撤销时还原父节点。  
- **列表操作**：记录卡牌在容器中的增删信息（如从储备区移除），撤销时恢复列表状态。  


#### 4. 示例：记录移动操作  
```cpp
// 创建移动操作记录
UndoAction action;
action.type = UndoActionType::MOVE_CARD;
action.card = movedCard;
action.originalPosition = movedCard->getPosition();
action.field = CardField::RESERVE; // 移动前位于储备区
action.id = undoManager.getNextUndoId();

// 执行撤销时使用
undoManager.undoLastAction(model, action);
```




### GameUtils 开发文档

#### 1. 类概述
`GameUtils` 是一个静态工具类，提供游戏开发中常用的辅助方法，目前主要支持字符编码转换功能。


#### 2. 核心方法
| 方法名           | 功能描述                                                                 |
|------------------|--------------------------------------------------------------------------|
| `GBKToUTF8()`    | 将GBK编码的字符串转换为UTF-8编码，解决中文显示乱码问题。                  |


#### 3. 使用示例
```cpp
// 将GBK编码的字符串转换为UTF-8
std::string gbkStr = "你好，世界";
std::string utf8Str = GameUtils::GBKToUTF8(gbkStr);

// 在Cocos2d-x中正确显示中文
Label* label = Label::createWithTTF(utf8Str, "fonts/arial.ttf", 24);
addChild(label);
```


#### 4. 注意事项
- 该方法主要用于解决Windows平台下中文字符显示问题




### CardView 开发文档  

#### 1. 类概述  
`CardView` 是扑克牌的视觉表现层，继承自Cocos2d-x的`Sprite`类，负责根据`CardModel`数据渲染卡牌的正面点数、花色及背面背景。  


#### 2. 核心职责  
- **视觉渲染**：根据模型状态（是否翻开、是否匹配）显示对应的图片资源。  
- **布局管理**：自动排版卡牌的背景、数字和花色图标位置。  
- **数据绑定**：通过`CardModel`获取卡牌属性（点数、花色、位置等）。  


#### 3. 关键属性  
| 属性名          | 类型         | 描述                          |  
|-----------------|--------------|-------------------------------|  
| `kCardWidth`    | float        | 卡牌宽度（固定值，默认120px） |  
| `kCardHeight`   | float        | 卡牌高度（固定值，默认180px） |  
| `model`         | CardModel*   | 关联的数据模型                |  
| `cardBgPath`    | std::string  | 背景图片路径（如背面图片）    |  


#### 4. 核心方法  
| 方法名              | 功能描述                                                                 |  
|---------------------|--------------------------------------------------------------------------|  
| `create(model, bgPath)` | 工厂方法，创建卡牌视图并绑定模型和背景路径。                             |  
| `renderCardBackground()` | 渲染卡牌背景（背面或正面底图），支持动态切换图片。                       |  
| `renderTopLeftNumber()`  | 在卡牌左上角显示小点数图标（如"A"、"10"）。                             |  
| `renderSuitIcon()`       | 根据模型花色渲染对应的图标（梅花、红桃等）。                             |  


#### 5. 使用示例  
```cpp
// 创建卡牌视图（传入模型和背面背景路径）
CardView* cardView = CardView::create(cardModel, "res/card_back.png");
addChild(cardView);

// 根据模型状态更新视图（如翻开卡牌）
if (cardModel->isCardFlipped()) {
    cardView->renderTopLeftNumber();
    cardView->renderSuitIcon();
}
```


#### 6. 资源依赖  
- **背景图片**：通过`cardBgPath`指定，通常为卡牌背面图片。  
- **数字图标**：通过`CardResConfig`获取小数字/大数字图片路径（如`small_red_A.png`）。  
- **花色图标**：通过`CardResConfig`获取对应花色图片（如`heart.png`）。




### GameView 开发文档  

#### 1. 类概述  
`GameView` 是游戏的视觉表现层，负责搭建游戏界面（如主区域、牌堆区域、撤销按钮），处理用户触摸事件，并通过回调函数将交互事件传递给控制器。  


#### 2. 核心职责  
- **界面搭建**：创建游戏区域和牌堆区域的节点结构，渲染卡牌视图。  
- **交互处理**：监听卡牌点击和撤销按钮点击事件，通过回调函数触发逻辑层响应。  
- **数据绑定**：关联`GameModel`获取卡牌布局数据，同步视图与模型状态。  


#### 3. 关键属性  
| 属性名                | 类型               | 描述                          |  
|-----------------------|--------------------|-------------------------------|  
| `model`               | GameModel*         | 关联的游戏数据模型            |  
| `mainAreaNode`        | Node*              | 主区域节点（放置可交互卡牌）  |  
| `pileAreaNode`        | Node*              | 牌堆区域节点（放置牌堆卡牌）  |  
| `_onCardClickCallback`| function<int()>    | 卡牌点击回调（参数为卡牌ID）  |  
| `_undoLabel`          | Label*             | 撤销按钮标签（显示“撤销”文本）|  


#### 4. 核心方法  
| 方法名                  | 功能描述                                                         |  
|-------------------------|------------------------------------------------------------------|  
| `initWithModel(model)`  | 用数据模型初始化视图，创建界面元素并绑定数据。                   |  
| `createGameAreas()`     | 创建主区域和牌堆区域的节点，设置布局参数（如位置、尺寸）。       |  
| `setupTouchHandling()`  | 注册触摸事件监听，处理卡牌和撤销按钮的点击检测。                 |  
| `setOnCardClickCallback()` | 设置卡牌点击回调，供控制器接收交互事件。                  |  


#### 5. 使用示例  
```cpp
// 创建游戏视图并绑定模型
GameView* gameView = GameView::create(gameModel);
addChild(gameView);

// 设置卡牌点击回调
gameView->setOnCardClickCallback([=](int cardId) {
    // 处理卡牌点击逻辑（如调用控制器方法）
    gameController->handleCardClick(cardId);
});

// 设置撤销回调
gameView->setOnUndoClickCallback([=]() {
    gameController->handleUndoClick();
});
```


#### 6. 交互流程  
1. 用户点击卡牌 → `GameView` 检测触摸位置 → 触发`_onCardClickCallback`回调。  
2. 用户点击撤销按钮 → 触发`_onUndoClickCallback`回调 → 通知控制器执行撤销逻辑。  
3. 视图通过`mainAreaNode`和`pileAreaNode`管理卡牌的层级和位置，自动同步模型数据。


### 新增卡牌与回退功能

#### 一、新增卡牌类型的步骤

在现有代码结构下，新增一种卡牌类型（如增加"王牌" Joker）需要以下步骤：

1. **扩展枚举定义**（修改`CardResConfig.h`）
```cpp
// 新增王牌类型
enum CardSuit {
    SUIT_CLUBS = 0,    // 梅花
    SUIT_DIAMONDS,     // 方块
    SUIT_HEARTS,       // 红桃
    SUIT_SPADES,       // 黑桃
    SUIT_JOKER         // 新增：王牌
};
```

2. **添加资源配置**（修改`CardResConfig.cpp`）
```cpp
// 为王牌添加资源路径规则
std::string CardResConfig::getSuitPath(CardSuit suit) {
    static const char* suitNames[] = { "club", "diamond", "heart", "spade", "joker" };
    return "res/suits/" + std::string(suitNames[suit]) + ".png";
}
```

3. **更新视图渲染逻辑**（修改`CardView.cpp`）
```cpp
void CardView::renderSuitIcon() {
    // 处理王牌特殊逻辑
    if (model->getSuit() == SUIT_JOKER) {
        // 渲染王牌图标（如特殊大小或位置）
    } else {
        // 原有逻辑
    }
}
```

4. **调整匹配规则**（修改`CardModel.cpp`）
```cpp
bool CardModel::canMatchWith(const CardModel* other) const {
    // 王牌特殊匹配规则（如可与任何牌匹配）
    if (this->getSuit() == SUIT_JOKER || other->getSuit() == SUIT_JOKER) {
        return true;
    }
    // 原有规则
    return abs(int(_face) - int(other->_face)) == 1;
}
```


#### 二、新增回退功能类型的步骤

新增一种回退操作（如"洗牌"操作的回退）需要以下步骤：

1. **扩展操作类型枚举**（修改`UndoManager.h`）
```cpp
enum class UndoActionType {
    MOVE_CARD,        // 卡牌移动
    CHANGE_PARENT,    // 父节点变更
    CHANGE_VECTOR,    // 容器变更
    SHUFFLE_DECK      // 新增：洗牌操作
};
```

2. **更新撤销操作结构体**（修改`UndoManager.h`）
```cpp
struct UndoAction {
    UndoActionType type;
    CardModel* card;
    // 新增洗牌操作所需字段
    std::vector<int> originalDeckOrder; // 洗牌前的牌堆顺序
    // 其他字段...
};
```

3. **实现具体回退逻辑**（修改`UndoManager.cpp`）
```cpp
void UndoManager::undoLastAction(GameModel* model) {
    if (_undoStack.empty()) return;
    
    const UndoAction& action = _undoStack.back();
    switch (action.type) {
        case UndoActionType::SHUFFLE_DECK:
            // 恢复洗牌前的牌堆顺序
            model->restoreDeckOrder(action.originalDeckOrder);
            break;
        // 其他类型...
    }
    _undoStack.pop_back();
}
```

4. **记录新操作**（在控制器中调用）
```cpp
void GameController::shuffleDeck() {
    // 记录洗牌前的状态
    UndoAction action;
    action.type = UndoActionType::SHUFFLE_DECK;
    action.originalDeckOrder = model->getCurrentDeckOrder();
    undoManager.recordAction(action);
    
    // 执行洗牌
    model->shuffleDeck();
}
```
