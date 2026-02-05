#ifndef WEBUI_H
#define WEBUI_H

#include "Constants.h"
#include "Patterns.h"
#include "Utils.h"
#include "Games.h"

const char WEB_PAGE[] PROGMEM = R"=====(
<html><head>
<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>
<style>
  body{background:#e8e3d8;color:#222;font-family:Arial, sans-serif;text-align:center;margin:0;padding-bottom:70px;}
  .modeBtn, .patBtn{padding:8px;margin:4px;border:none;border-radius:8px;background:#d6d0c4;font-size:12px;cursor:pointer;transition: 0.2s;}
  .modeBtn.active, .patBtn.active{background:#7ea88f; color: white; font-weight:bold;}
  .panel{display:none;background:#f4f1ea;border:1px solid #bbb;padding:10px;border-radius:10px;margin-top:10px;}
  .grid{display:grid;grid-template-columns:repeat(16,22px);gap:3px;justify-content:center;touch-action:none;user-select:none;}
  .cell{width:22px;height:22px;background:#fff;border:1px solid #aaa;touch-action:none;}
  select, input[type='text'], input[type='number']{padding:8px; border-radius:5px; border:1px solid #ccc; margin:5px;}
  .joy{display:grid;grid-template-columns:repeat(3,70px);gap:10px;justify-content:center;margin-top:15px;}
  .joy button{width:70px;height:70px;border-radius:50%;border:none;background:#7ea88f;color:white;font-size:20px;}
  .watermark{position:fixed;bottom:0;left:0;width:100%;background:#222;color:#7ea88f;padding:10px 0;font-weight:bold;z-index:9999;}
</style></head><body>

<h2>ESP32 LED Matrix</h2>

<div>
  <button id='m0' class='modeBtn' onclick='setMode(0,this)'>Display</button>
  <button id='m1' class='modeBtn' onclick='setMode(1,this)'>Clock</button>
  <button id='m2' class='modeBtn' onclick='setMode(2,this)'>Games</button>
  <button id='m3' class='modeBtn' onclick='setMode(3,this)'>Robot</button>
</div>

<p>Brightness</p>
<input type='range' min='10' max='150' value='40' oninput='fetch(`/set?b=`+this.value)'>

<div id='display' class='panel'>
  <p>Speed</p>
  <input type='range' min='0' max='100' value='50' oninput='fetch(`/set?s=`+this.value)'>
  <p>Pattern Selection</p>
  <div style='display:grid;grid-template-columns:repeat(5,1fr);gap:6px;'>
    <button class='patBtn' onclick='setPattern(0,this)'>🎨 Draw</button>
    <button class='patBtn' onclick='setPattern(1,this)'>🖼 Image</button>
    <button class='patBtn' onclick='setPattern(2,this)'>📝 Text</button>
    <button class='patBtn' onclick='setPattern(3,this)'>🌧 Rain</button>
    <button class='patBtn' onclick='setPattern(4,this)'>🎆 Firework</button>
    <button class='patBtn' onclick='setPattern(5,this)'>🧱 Tetris</button>
    <button class='patBtn' onclick='setPattern(6,this)'>🌈 Aurora</button>
    <button class='patBtn' onclick='setPattern(7,this)'>🔥 Fire</button>
    <button class='patBtn' onclick='setPattern(8,this)'>✨ Glitter</button>
    <button class='patBtn' onclick='setPattern(9,this)'>🌊 Ocean</button>
    <button class='patBtn' onclick='setPattern(10,this)'>☄ Meteor</button>
    <button class='patBtn' onclick='setPattern(11,this)'>🌌 Milky</button>
    <button class='patBtn' onclick='setPattern(12,this)'>💓 Pulse</button>
    <button class='patBtn' onclick='setPattern(13,this)'>🌀 Spiral</button>
    <button class='patBtn' onclick='setPattern(14,this)'>🌀 Vortex</button>
    <button class='patBtn' onclick='setPattern(15,this)'>🌌 Galaxy</button>
    <button class='patBtn' onclick='setPattern(16,this)'>🟩 Matrix</button>
    <button class='patBtn' onclick='setPattern(17,this)'>🏀 Balls</button>
    <button class='patBtn' onclick='setPattern(18,this)'>🌋 Lava</button>
    <button class='patBtn' onclick='setPattern(19,this)'>⭕ Rings</button>
  </div>

  <div id='drawPanel' class='panel'>
    <input type='color' id='color' value='#ff0000'>
    <button id='brushBtn' class='patBtn active' onclick='setTool(0,this)'>Brush</button>
    <button id='eraserBtn' class='patBtn' onclick='setTool(1,this)'>Eraser</button>
    <button class='patBtn' onclick='clearGrid()'>Clear</button>
    <div class='grid' id='grid'></div>
  </div>

  <div id='imagePanel' class='panel'>
    <input type='file' id='img' accept='image/*'>
    <canvas id='cv' width='16' height='16' style='display:none'></canvas>
    <div class='grid' id='previewGrid'></div>
    <button class='modeBtn active' onclick='sendImage()'>Send Image</button>
  </div>

  <div id='textPanel' class='panel'>
    <input id='textInput' style='width:80%;' placeholder='Enter text...'>
    <br>
    <label>Color:</label> <input type='color' id='textColor' value='#00ffff'>
    <button class='patBtn' onclick='sendTextColor()'>Set</button>
    <button class='patBtn' onclick='fetch("/textcolor?rand=1")'>🎲 Rand</button>
    <br>
    <label>Position:</label>
    <select id='textPos' onchange='fetch("/textpos?y="+this.value)'>
      <option value='0'>Top</option>
      <option value='5' selected>Middle</option>
      <option value='11'>Bottom</option>
    </select>
    <br>
    <button class='modeBtn active' style='width:80%;' onclick='sendText()'>Update Text</button>
  </div>

  <div id='ringCtrl' class='panel'>
    <p><b>⭕ Ring Settings</b></p>
    <input type='color' id='ringColor' value='#00ffff'>
    <button class='patBtn' onclick='setRingColor()'>Set</button>
    <button class='patBtn' onclick='fetch("/ringcolor?auto=1")'>🌈 Auto</button>
  </div>

  <div id='ballCtrl' class='panel'>
    <p><b>🏀 Ball Count</b></p>
    <input type='number' min='1' max='12' value='6' onchange='fetch("/ballcount?n="+this.value)'>
  </div>
</div>

<div id='clock' class='panel'>
  <p>Digital Clock Mode Active</p>
  <p style='font-size: 12px; color: #666;'>Time is automatically synced via NTP.</p>
</div>

<div id='robot' class='panel'>
  <p>Robot Expressions</p>
  <div style='display:grid; grid-template-columns: repeat(3, 1fr); gap: 10px;'>
    <button class='patBtn' onclick='fetch("/robot?e=0")'>😐 Neutral</button>
    <button class='patBtn' onclick='fetch("/robot?e=1")'>😊 Happy</button>
    <button class='patBtn' onclick='fetch("/robot?e=2")'>😠 Angry</button>
    <button class='patBtn' onclick='fetch("/robot?e=3")'>😮 Surprise</button>
    <button class='patBtn' onclick='fetch("/robot?e=4")'>❤️ Love</button>
  </div>
</div>

<div id='game' class='panel'>
  <p>Select Game:</p>
  <select onchange='fetch("/selectGame?g="+this.value)' style='padding:10px; width:80%; border-radius:8px;'>
    <option value='0'>Snake</option>
    <option value='1'>Flappy Bird</option>
    <option value='2'>Breakout</option>
    <option value='3'>Space Invaders</option>
    <option value='4'>Dino Run</option>
  </select>

  <div class='joy'>
    <div></div><button onclick='fetch("/gameAction?d=U")'>▲</button><div></div>
    <button onclick='fetch("/gameAction?d=L")'>◀</button>
    <button onclick='fetch("/gameAction?d=D")'>▼</button>
    <button onclick='fetch("/gameAction?d=R")'>▶</button>
  </div>
</div>

<div class="watermark">DEVELOPED BY PRADUM PAL</div>

<script>
  let drawing=false, tool=0;

  function setMode(m, btn) {
    fetch('/mode?m='+m);
    document.querySelectorAll('.modeBtn').forEach(b=>b.classList.remove('active'));
    if(btn) btn.classList.add('active');
    document.getElementById('display').style.display = (m==0)?'block':'none';
    document.getElementById('clock').style.display = (m==1)?'block':'none';
    document.getElementById('game').style.display = (m==2)?'block':'none';
    document.getElementById('robot').style.display = (m==3)?'block':'none';
  }

  function setPattern(p, btn) {
    fetch('/set?p='+p);
    document.querySelectorAll('.patBtn').forEach(b=>b.classList.remove('active'));
    if(btn) btn.classList.add('active');
    document.getElementById('drawPanel').style.display = (p==0)?'block':'none';
    document.getElementById('imagePanel').style.display = (p==1)?'block':'none';
    document.getElementById('textPanel').style.display = (p==2)?'block':'none';
    document.getElementById('ringCtrl').style.display = (p==19)?'block':'none';
    document.getElementById('ballCtrl').style.display = (p==17)?'block':'none';
  }

  const grid = document.getElementById('grid');
  for(let i=0; i<256; i++){
    let c = document.createElement('div');
    c.className = 'cell';
    c.dataset.idx = i;
    c.onmousedown = (e) => { drawing=true; paint(c, i); e.preventDefault(); };
    c.onmouseenter = () => { if(drawing) paint(c, i); };
    grid.appendChild(c);
  }
  window.onmouseup = () => drawing = false;

  grid.addEventListener('touchstart', (e) => { drawing = true; handleTouch(e); e.preventDefault(); }, {passive: false});
  grid.addEventListener('touchmove', (e) => { if(drawing) handleTouch(e); e.preventDefault(); }, {passive: false});
  grid.addEventListener('touchend', () => drawing = false);

  function handleTouch(e) {
    let touch = e.touches[0];
    let el = document.elementFromPoint(touch.clientX, touch.clientY);
    if (el && el.classList.contains('cell')) paint(el, parseInt(el.dataset.idx));
  }

  function setTool(t,btn){ tool=t; brushBtn.classList.remove('active'); eraserBtn.classList.remove('active'); btn.classList.add('active'); }
  
  function paint(cell,i){
    let r=0,g=0,b=0;
    if(tool==0){
      let col = document.getElementById('color').value; cell.style.background=col;
      r=parseInt(col.substr(1,2),16); g=parseInt(col.substr(3,2),16); b=parseInt(col.substr(5,2),16);
    } else cell.style.background='#fff';
    fetch(`/draw?x=${i%16}&y=${Math.floor(i/16)}&r=${r}&g=${g}&b=${b}`);
  }

  function clearGrid(){ fetch('/clear'); document.querySelectorAll('#grid .cell').forEach(c=>c.style.background='#fff'); }

  const pGrid = document.getElementById('previewGrid');
  for(let i=0;i<256;i++){ let d=document.createElement('div'); d.className='cell'; pGrid.appendChild(d); }
  img.onchange=(e)=>{
    let reader = new FileReader();
    reader.onload=(ev)=>{
      let im=new Image(); im.onload=()=>{
        let ctx=cv.getContext('2d'); ctx.drawImage(im,0,0,16,16);
        let d=ctx.getImageData(0,0,16,16).data;
        document.querySelectorAll('#previewGrid .cell').forEach((c,i)=>{ c.style.background=`rgb(${d[i*4]},${d[i*4+1]},${d[i*4+2]})`; });
      }; im.src=ev.target.result;
    }; reader.readAsDataURL(e.target.files[0]);
  };

  function sendImage(){
    let d=cv.getContext('2d').getImageData(0,0,16,16).data; let out='';
    for(let i=0;i<d.length;i+=4) out+=d[i]+','+d[i+1]+','+d[i+2]+',';
    fetch('/frame?data='+out);
  }

  function sendText(){ fetch('/text?t='+encodeURIComponent(textInput.value)); }
  function sendTextColor(){
    let col = textColor.value;
    fetch(`/textcolor?r=${parseInt(col.substr(1,2),16)}&g=${parseInt(col.substr(3,2),16)}&b=${parseInt(col.substr(5,2),16)}`);
  }
  function setRingColor(){ let c=ringColor.value; fetch(`/ringcolor?r=${parseInt(c.substr(1,2),16)}&g=${parseInt(c.substr(3,2),16)}&b=${parseInt(c.substr(5,2),16)}`); }

  setMode(0, document.getElementById('m0'));
</script>
</body></html>
)=====";

void setupWebHandlers() {
  server.on("/", [](){ server.send_P(200, "text/html", WEB_PAGE); });
  server.on("/mode", [](){ uiMode = server.arg("m").toInt(); gameRunning = false; server.send(200); });
  server.on("/set", []() {
    if(server.hasArg("b")) { brightness = server.arg("b").toInt(); strip.setBrightness(brightness); prefs.putInt("bright", brightness); }
    if(server.hasArg("p")) { currentPattern = server.arg("p").toInt(); prefs.putInt("pattern", currentPattern); }
    if(server.hasArg("s")) { speedValue = server.arg("s").toInt(); prefs.putInt("speed", speedValue); }
    server.send(200);
  });
  server.on("/selectGame", [](){ selectedGame = server.arg("g").toInt(); gameRunning = false; server.send(200); });
  server.on("/gameAction", [](){ handleGameInput(server.arg("d")); server.send(200); });
  server.on("/robot", [](){ if(server.hasArg("e")) robotExpression = server.arg("e").toInt(); server.send(200); });
  server.on("/draw", []() {
    int x = server.arg("x").toInt(); int y = server.arg("y").toInt();
    int r = server.arg("r").toInt(); int g = server.arg("g").toInt(); int b = server.arg("b").toInt();
    strip.setPixelColor(getPhysicalLED(y*COLS+x), strip.Color(r,g,b)); strip.show();
    server.send(200);
  });
  server.on("/clear", []() { strip.clear(); strip.show(); server.send(200); });
  server.on("/text", []() { scrollText = server.arg("t"); scrollX = COLS; server.send(200); });
  server.on("/textpos", []() { if(server.hasArg("y")) scrollY = server.arg("y").toInt(); server.send(200); });
  server.on("/textcolor", []() {
    if(server.hasArg("r")) { scrollColor = strip.Color(server.arg("r").toInt(), server.arg("g").toInt(), server.arg("b").toInt()); randomScrollColor = false; }
    if(server.hasArg("rand")) randomScrollColor = true;
    server.send(200);
  });
  server.on("/ringcolor", []() {
    if(server.hasArg("auto")) ringAutoColor = true;
    else { ringColor = strip.Color(server.arg("r").toInt(), server.arg("g").toInt(), server.arg("b").toInt()); ringAutoColor = false; }
    server.send(200);
  });
  server.on("/ballcount", []() { ballCount = server.arg("n").toInt(); server.send(200); });
  server.on("/frame", []() {
    String data = server.arg("data");
    int idx = 0, led = 0;
    while (idx < data.length() && led < NUM_LEDS) {
      int r = data.substring(idx, data.indexOf(',', idx)).toInt(); idx = data.indexOf(',', idx) + 1;
      int g = data.substring(idx, data.indexOf(',', idx)).toInt(); idx = data.indexOf(',', idx) + 1;
      int b = data.substring(idx, data.indexOf(',', idx)).toInt(); idx = data.indexOf(',', idx) + 1;
      strip.setPixelColor(getPhysicalLED(led++), strip.Color(r,g,b));
    }
    strip.show();
    server.send(200);
  });
}
#endif