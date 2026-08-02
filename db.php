<?php
declare(strict_types=1);

/*
 * ضعي كلمة مرور MySQL الحقيقية في $pass.
 * لا ترفعي كلمة المرور الحقيقية إلى GitHub عام.
 */
$host = "sql110.infinityfree.com";
$user = "if0_42550058";
$pass = "PUT_YOUR_MYSQL_PASSWORD_HERE";
$dbname = "if0_42550058_robot_control_db";
$port = 3306;

mysqli_report(MYSQLI_REPORT_OFF);
$conn = new mysqli($host, $user, $pass, $dbname, $port);

if ($conn->connect_errno) {
    http_response_code(500);
    die("Database connection failed.");
}
if (!$conn->set_charset("utf8mb4")) {
    http_response_code(500);
    die("Could not set database character set.");
}
