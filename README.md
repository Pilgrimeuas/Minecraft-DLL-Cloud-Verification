————中文教程：
一个狗屎验证，仅供参考：，
Client内容放到你的项目里面，建议放到modulemanager，
先把void veri(){...}写好，(配置ip和端口之类的)，
然后再把veri(); 加到下面的setenable（）下面一行那里,
每次注入都会触发一次,
Server内容是要生成exe（需要你重新开个项目或者用clang什么都行），
要把hwid也就是白名单用户库写好(可以读取txt文本，或者在cpp里直接写)然后生成，
然后把exe拖到服务器，开放端口，然后打开exe，就可以验证了，


————English  Idiot Usage：(if not , just skip it)，
Client side:，
1.the shit in Client.cpp should be put in your dll proj and i recommend u put it in Modulemanager，
2.intially set your ip or port，
3.then wirte a veri() in initModules() [i think one is enough]，
，
Server side:，
1.wirte your hwid WhiteList in cpp and remember to set port，
2.build Server.cpp into exe，
3. put exe into your server and run it，
