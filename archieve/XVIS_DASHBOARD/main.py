import time
import json
from http.server import ThreadingHTTPServer, BaseHTTPRequestHandler
import vcom

ser = vcom.open_serial()

PAGE = b"""<body style="background:#111;color:#fff;font-family:monospace;display:flex;flex-direction:column;align-items:center;justify-content:center;height:95vh">
<div style="margin-bottom:8px">
  <button id="btn" onclick="toggle()" style="background:#222;color:#0f0;padding:6px 16px;border:1px solid #0f0;cursor:pointer">STOP</button>
</div>
<canvas id="c" width="500" height="300" style="background:#000;border:1px solid #333"></canvas>
<script>
const ctx = document.getElementById("c").getContext("2d");
let pts = [];
let on = 1;

function toggle() {
  on = on ? 0 : 1;
  fetch("/cmd?c=" + on);
  document.getElementById("btn").innerText = on ? "STOP" : "START";
}

new EventSource("/s").onmessage = e => {
  const [a, d] = JSON.parse(e.data);
  const r = a * Math.PI / 180;
  pts.push({x: 250 + d * Math.cos(r), y: 280 - d * Math.sin(r), t: Date.now()});
};

function draw() {
  const now = Date.now();
  pts = pts.filter(p => now - p.t < 4000);
  ctx.clearRect(0, 0, 500, 300);
  ctx.fillStyle = "#0f0";
  for (const p of pts) {
    ctx.fillRect(p.x, p.y, 4, 4);
  }
  requestAnimationFrame(draw);
}
draw();
</script></body>"""

class RadarHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path.startswith("/cmd"):
            cmd = self.path.split("=")[-1]
            vcom.send_cmd(ser, cmd)
            self.send_response(200)
            self.end_headers()
            return
        if self.path == "/s":
            self.send_response(200)
            self.send_header("Content-Type", "text/event-stream")
            self.end_headers()
            while True:
                pt = vcom.read_sample(ser)
                if pt:
                    self.wfile.write(f"data: {json.dumps(pt)}\n\n".encode())
                    self.wfile.flush()
                time.sleep(0.02)
            return
        self.send_response(200)
        self.end_headers()
        self.wfile.write(PAGE)

if __name__ == "__main__":
    ThreadingHTTPServer(("0.0.0.0", 8080), RadarHandler).serve_forever()
