const char fileBrowserPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<body>
<h2>Door Monitor File Browser</h2>
<div id="list"></div>

<script>
fetch('/list')
  .then(response => response.text())
  .then(text => {
    const lines = text.trim().split('\n');
    let html = "";
    lines.forEach(line => {
      const [name, size] = line.split(',');
      html += `
      <div style="display:flex; align-items:center; gap:12px; margin-bottom:4px;">
        <button class="delete-btn" onclick="deleteFile('/${name}')">x</button>
        <a href="/download?file=/${name}">${name}</a>
        <span style="color:#666; font-size:12px;">${size} bytes</span>
      </div>
      `;
    });
    document.getElementById('list').textContent = "";
    document.getElementById('list').innerHTML = html.replace(/\n/g, "<br>");
  });

function deleteFile(path) {
  if (!confirm("Delete " + path + "?")) return;

  fetch('/delete?file=' + encodeURIComponent(path))
    .then(res => res.text())
    .then(msg => {
      alert(msg);
      location.reload();  // refresh file list
    });
}
</script>
<style>
.delete-btn {
  padding: 2px 6px;
  font-size: 12px;
  cursor: pointer;
  background: #eee;
  border: 1px solid #ccc;
  border-radius: 4px;
}
.delete-btn:hover {
  background: #ffcccc;
  border-color: #ff4444;
}
</style>
</body>
</html>
)rawliteral";
