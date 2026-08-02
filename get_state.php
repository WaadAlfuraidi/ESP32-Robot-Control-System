<?php
declare(strict_types=1);
header("Content-Type: text/plain; charset=UTF-8");
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Pragma: no-cache");
header("Expires: 0");
require_once __DIR__ . "/db.php";

$result=$conn->query("SELECT command FROM robot_state WHERE id=1 LIMIT 1");
if (!$result || $result->num_rows===0) {
  echo "S";$conn->close();exit;
}
$row=$result->fetch_assoc();
$command=strtoupper(trim((string)($row["command"]??"S")));
echo in_array($command,["F","B","L","R","S"],true)?$command:"S";
$conn->close();
