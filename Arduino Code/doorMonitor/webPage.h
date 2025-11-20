const char fileBrowserPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<body>
<h2>SD Card File Browser</h2>
<pre id="list"></pre>

<script>
fetch('/list')
  .then(response => response.text())
  .then(text => {
    const lines = text.trim().split('\n');
    let html = "";
    lines.forEach(line => {
      const [name, size] = line.split(',');
      html += `<a href="/download?file=/${name}">${name}</a> (${size} bytes)
      <button onclick="deleteFile('/${name}')">Delete</button>\n`;
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
</body>
</html>
)rawliteral";
