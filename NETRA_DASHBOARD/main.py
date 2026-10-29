import time, json
from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
import vcom
import slam

ser = vcom.open_serial()

class RadarHandler(SimpleHTTPRequestHandler):
    def log_message(self, format, *args):
        return

    def do_GET(self):
        if self.path == "/s":
            self.send_response(200)
            self.send_header("Content-Type", "text/event-stream")
            self.send_header("Cache-Control", "no-cache")
            self.send_header("Connection", "keep-alive")
            self.end_headers()
            while True:
                pt = vcom.read_latest(ser)
                if pt:
                    dists = pt[0]
                    yaw = pt[2]["yaw"] if len(pt) > 2 and isinstance(pt[2], dict) else 0.0
                    slam_map = slam.update_slam(dists, yaw)
                    payload = json.dumps({"pt": pt, "slam": slam_map})
                    self.wfile.write(f"data: {payload}\n\n".encode())
                    self.wfile.flush()
                else:
                    time.sleep(0.005)
            return

        if self.path.startswith("/cmd"):
            cmd = self.path.split("=")[-1]
            if cmd == "reset_slam":
                slam.reset_slam()
            else:
                vcom.send_cmd(ser, cmd)
            self.send_response(200)
            self.end_headers()
            return

        return super().do_GET()

if __name__ == "__main__":
    ThreadingHTTPServer(("0.0.0.0", 8080), RadarHandler).serve_forever()
