-- شغلي هذا الملف من phpMyAdmin داخل قاعدة البيانات.
CREATE TABLE IF NOT EXISTS robot_state (
  id INT NOT NULL,
  command CHAR(1) NOT NULL DEFAULT 'S',
  updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

INSERT INTO robot_state (id, command)
VALUES (1, 'S')
ON DUPLICATE KEY UPDATE command=VALUES(command), updated_at=CURRENT_TIMESTAMP;

SELECT * FROM robot_state;
