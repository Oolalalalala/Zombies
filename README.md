# 程城波-Zombies

## 小問題
痾，只能用Visual studio我也沒辦法，不然會更麻煩(因為3D要用其他函示庫，我不會其他IDE的設定)。如果電腦沒下載的話，先去下載Visual Studio最好2022版。

## How to build
建議用`git clone`下載，這樣以後上傳比較方便，如果電腦上還沒有git先去[下載](https://git-scm.com/download/win)。打開要放程式的資料夾後，直接在路徑打入`cmd`，就會開啟一個黑色視窗，打下面這行指令進行下載。
```
git clone https://github.com/Oolalalalala/Zombies.git。
```
下載程式碼後點`BuildProject.bat`兩下後專案就會建好，將.sln檔打開就會跳出Visual Studio。進去後先按F5編譯，他會說沒有dll沒辦法執行，這時候去叫dll的資料夾，把對應的兩個dll複製到.exe檔旁邊(記得選x64編譯，x32壞掉)，至於.exe在哪裡了呢，用不同的設定編譯就會產生不同的執行檔，在bin資料夾裡可以找到對應的.exe。再按一次F5理論上就可以執行了。

## 寫程式?
要寫遊戲邏輯的話全部都寫在`Game/src`底下，我在`Game.h`和`Game.cpp`已經寫一些示範的程式碼了，看不懂正常，但應該可以看函式的名子就知道他在幹嘛了，看不懂就問我。

## 程式小介紹
1. `OnUpdate(float dt)`:每幀更新一次，更新的程式碼就寫在這裡
2. `Scene`是很重要的一個class，要創造一個實體呼叫`Scene.CreateEntity()`，`Scene.Update()`會把所有Scene裡的實體畫出來
3. `Entity`擁有很多component，創造時會預設加入`TransformComponent`包含位置、轉向、大小比例，`entity.AddComponent<T>()`會創造並回傳Component，`entity.GetComponent<T>()`會回傳`Component`，如果要改變回傳的`Component`，記得要`&`回傳值。
4. `Ref<T>`是什麼呢，他其實就是`shared_ptr<T>`，可以上網查，基本上就是自動管理的指標，可以把它當作一般的指標，但假如沒有任何指標指向這個位置的時候他會自己`delete`，`CreateRef<T>()`會創造一個`T`的`Class`，括號裡的參數跟`T`的`Constructor`一樣。
5. `IO`的使用看我寫的範例，應該看得懂。
6. `glm`是一個數學函示庫，記得要用`glm::vec3`這種namespace的方式使用，基本的向量數學都有。
7. `CameraComponent`鏡頭，表示看東西的出發點，改`TransformComponent`的位置和角度可以改變視角，切記不要去用`CameraComponent::Camera::SetDirection()`這個函式，他會被`Trasform`的角度覆蓋掉。
8. `AssetManager::LoadTexture(std::string)`會載入一個2D圖片，`AssetManager::LoadModel(std::string)`則會載入一個3D模型，他們都會回傳`AssetHandle`可以塞進`SpriteRendererComponent`和`ModelRendererComponent`他們就會出現在螢幕上了。


## 要怎麼上傳到Github呢
建議用Git，不然會多傳到很多東西，如果使用Git，在Zombies的資料夾開啟cmd，照下面的指令打，就上傳完成了。
```
git init // 如果用clone就不用這行
git add .
git commit -m "YourMessageHere"
git remote add origin https://github.com/Oolalalalala/Zombies.git  // 如果用clone就不用這行
git push -u origin main
```
如果不是用Git，記得.gitignore檔裡所列出的檔案不要上傳😡。

## TODO:
1. Add support for audio
2. Add support for text rendering
3. Fix messy model rendering code
