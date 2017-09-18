**JSC.js** is a JavaScript engine which can run on top of you browser's JavaScript engine. It's based on the default JavaScript engine of WebKit. Therefore, if you're using Safari, you can literally run its JavaScript engine on top of itself.

About *JavaScriptCore*  
JavaScriptCore is the name of the default JavaScript engine for WebKit. Now it is compiled into WebAssembly with emscripten.

The size of JSC.js is around 3MB (compressed js and mem file).

## Demo: [Link](https://mbbill.github.io/JSC.js/demo/index.html)

## ScreenShot
![](https://sites.google.com/site/mbbill/jsc3.png)

# Build
- install emscripten
- install python, ruby, etc.
- run `.\Source\DerivedSources\gen.bat` 
- run build.bat

