<?php
declare(strict_types=1);
header("Content-Type: application/json; charset=UTF-8");
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
require_once __DIR__ . "/db.php";

$raw = strtoupper(trim((string)($_POST["command"] ?? $_GET["command"] ?? "")));
$map = [
  "F"=>"F","FORWARD"=>"F","1"=>"F",
  "B"=>"B","BACKWARD"=>"B","2"=>"B",
  "L"=>"L","LEFT"=>"L","3"=>"L",
  "R"=>"R","RIGHT"=>"R","4"=>"R",
  "S"=>"S","STOP"=>"S","0"=>"S"
];

if (!isset($map[$raw])) {
  http_response_code(400);
  echo json_encode(["success"=>false,"message"=>"Invalid command."], JSON_UNESCAPED_UNICODE);
  $conn->close();
  exit;
}

$command=$map[$raw];
$sql="INSERT INTO robot_state (id, command) VALUES (1, ?)
      ON DUPLICATE KEY UPDATE command=VALUES(command), updated_at=CURRENT_TIMESTAMP";
$stmt=$conn->prepare($sql);

if (!$stmt) {
  http_response_code(500);
  echo json_encode(["success"=>false,"message"=>"Could not prepare query."], JSON_UNESCAPED_UNICODE);
  $conn->close();
  exit;
}

$stmt->bind_param("s",$command);
if (!$stmt->execute()) {
  http_response_code(500);
  echo json_encode(["success"=>false,"message"=>"Could not save command."], JSON_UNESCAPED_UNICODE);
  $stmt->close();$conn->close();exit;
}

echo json_encode(["success"=>true,"command"=>$command], JSON_UNESCAPED_UNICODE);
$stmt->close();$conn->close();
