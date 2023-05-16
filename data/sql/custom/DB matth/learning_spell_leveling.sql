/*
 Navicat Premium Data Transfer

 Source Server         : Valanior
 Source Server Type    : MySQL
 Source Server Version : 80032
 Source Host           : 151.80.58.21:3306
 Source Schema         : acore_world

 Target Server Type    : MySQL
 Target Server Version : 80032
 File Encoding         : 65001

 Date: 25/03/2023 18:26:28
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for learning_spell_leveling
-- ----------------------------
DROP TABLE IF EXISTS `learning_spell_leveling`;
CREATE TABLE `learning_spell_leveling`  (
  `classId` int(0) NOT NULL,
  `spellId` int(0) NULL DEFAULT NULL,
  `level` int(0) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of learning_spell_leveling
-- ----------------------------
INSERT INTO `learning_spell_leveling` VALUES (8, 43017, 18);
INSERT INTO `learning_spell_leveling` VALUES (8, 42897, 8);
INSERT INTO `learning_spell_leveling` VALUES (8, 1953, 20);
INSERT INTO `learning_spell_leveling` VALUES (8, 42940, 20);
INSERT INTO `learning_spell_leveling` VALUES (8, 42931, 26);
INSERT INTO `learning_spell_leveling` VALUES (8, 42956, 2);
INSERT INTO `learning_spell_leveling` VALUES (8, 2139, 24);
INSERT INTO `learning_spell_leveling` VALUES (8, 43015, 12);
INSERT INTO `learning_spell_leveling` VALUES (8, 12051, 20);
INSERT INTO `learning_spell_leveling` VALUES (8, 42873, 6);
INSERT INTO `learning_spell_leveling` VALUES (8, 43010, 20);
INSERT INTO `learning_spell_leveling` VALUES (8, 42926, 16);
INSERT INTO `learning_spell_leveling` VALUES (8, 42917, 10);
INSERT INTO `learning_spell_leveling` VALUES (8, 43012, 22);
INSERT INTO `learning_spell_leveling` VALUES (8, 42842, 4);
INSERT INTO `learning_spell_leveling` VALUES (8, 45438, 30);
INSERT INTO `learning_spell_leveling` VALUES (8, 66, 38);
INSERT INTO `learning_spell_leveling` VALUES (8, 43024, 34);
INSERT INTO `learning_spell_leveling` VALUES (8, 43020, 20);
INSERT INTO `learning_spell_leveling` VALUES (8, 43046, 52);
INSERT INTO `learning_spell_leveling` VALUES (8, 12826, 8);
INSERT INTO `learning_spell_leveling` VALUES (8, 475, 18);
INSERT INTO `learning_spell_leveling` VALUES (8, 42859, 22);
INSERT INTO `learning_spell_leveling` VALUES (8, 31589, 50);
INSERT INTO `learning_spell_leveling` VALUES (8, 30449, 60);
INSERT INTO `learning_spell_leveling` VALUES (8, 42927, 60);
INSERT INTO `learning_spell_leveling` VALUES (1, 18499, 32);
INSERT INTO `learning_spell_leveling` VALUES (1, 2458, 10);
INSERT INTO `learning_spell_leveling` VALUES (1, 1161, 26);
INSERT INTO `learning_spell_leveling` VALUES (1, 11578, 4);
INSERT INTO `learning_spell_leveling` VALUES (1, 47520, 20);
INSERT INTO `learning_spell_leveling` VALUES (1, 47440, 58);
INSERT INTO `learning_spell_leveling` VALUES (1, 71, 10);
INSERT INTO `learning_spell_leveling` VALUES (1, 47437, 14);
INSERT INTO `learning_spell_leveling` VALUES (1, 676, 18);
INSERT INTO `learning_spell_leveling` VALUES (1, 55694, 55);
INSERT INTO `learning_spell_leveling` VALUES (1, 47471, 24);
INSERT INTO `learning_spell_leveling` VALUES (1, 1715, 8);
INSERT INTO `learning_spell_leveling` VALUES (1, 57755, 60);
INSERT INTO `learning_spell_leveling` VALUES (1, 61241, 52);
INSERT INTO `learning_spell_leveling` VALUES (1, 5246, 22);
INSERT INTO `learning_spell_leveling` VALUES (1, 694, 16);
INSERT INTO `learning_spell_leveling` VALUES (1, 12323, 20);
INSERT INTO `learning_spell_leveling` VALUES (1, 6552, 38);
INSERT INTO `learning_spell_leveling` VALUES (1, 47465, 4);
INSERT INTO `learning_spell_leveling` VALUES (1, 20230, 20);
INSERT INTO `learning_spell_leveling` VALUES (1, 80011, 51);
INSERT INTO `learning_spell_leveling` VALUES (1, 72, 12);
INSERT INTO `learning_spell_leveling` VALUES (1, 2565, 16);
INSERT INTO `learning_spell_leveling` VALUES (1, 47488, 40);
INSERT INTO `learning_spell_leveling` VALUES (1, 47475, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 23920, 44);
INSERT INTO `learning_spell_leveling` VALUES (1, 7386, 10);
INSERT INTO `learning_spell_leveling` VALUES (1, 355, 10);
INSERT INTO `learning_spell_leveling` VALUES (1, 47502, 6);
INSERT INTO `learning_spell_leveling` VALUES (1, 34428, 6);
INSERT INTO `learning_spell_leveling` VALUES (1, 1680, 36);
INSERT INTO `learning_spell_leveling` VALUES (1, 47436, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 2457, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 47475, 1);
INSERT INTO `learning_spell_leveling` VALUES (8, 43002, 1);
INSERT INTO `learning_spell_leveling` VALUES (8, 42833, 1);
INSERT INTO `learning_spell_leveling` VALUES (8, 43008, 1);
INSERT INTO `learning_spell_leveling` VALUES (8, 85000, 1);
INSERT INTO `learning_spell_leveling` VALUES (8, 42921, 14);
INSERT INTO `learning_spell_leveling` VALUES (1, 674, 10);
INSERT INTO `learning_spell_leveling` VALUES (1, 3411, 60);
INSERT INTO `learning_spell_leveling` VALUES (1, 3127, 8);
INSERT INTO `learning_spell_leveling` VALUES (1, 750, 40);
INSERT INTO `learning_spell_leveling` VALUES (8, 42985, 28);
INSERT INTO `learning_spell_leveling` VALUES (2, 80058, 44);
INSERT INTO `learning_spell_leveling` VALUES (2, 31884, 26);
INSERT INTO `learning_spell_leveling` VALUES (2, 4987, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 19746, 22);
INSERT INTO `learning_spell_leveling` VALUES (2, 48819, 6);
INSERT INTO `learning_spell_leveling` VALUES (2, 32223, 60);
INSERT INTO `learning_spell_leveling` VALUES (2, 35395, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 48942, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 20216, 30);
INSERT INTO `learning_spell_leveling` VALUES (2, 19752, 30);
INSERT INTO `learning_spell_leveling` VALUES (2, 54428, 60);
INSERT INTO `learning_spell_leveling` VALUES (2, 498, 26);
INSERT INTO `learning_spell_leveling` VALUES (2, 642, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 80047, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 48801, 20);
INSERT INTO `learning_spell_leveling` VALUES (2, 48947, 36);
INSERT INTO `learning_spell_leveling` VALUES (2, 48785, 4);
INSERT INTO `learning_spell_leveling` VALUES (2, 48945, 32);
INSERT INTO `learning_spell_leveling` VALUES (2, 25898, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 48934, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 48938, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 10308, 5);
INSERT INTO `learning_spell_leveling` VALUES (2, 48806, 24);
INSERT INTO `learning_spell_leveling` VALUES (2, 1044, 18);
INSERT INTO `learning_spell_leveling` VALUES (2, 10278, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 62124, 14);
INSERT INTO `learning_spell_leveling` VALUES (2, 6940, 46);
INSERT INTO `learning_spell_leveling` VALUES (2, 1038, 26);
INSERT INTO `learning_spell_leveling` VALUES (2, 48782, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 48817, 50);
INSERT INTO `learning_spell_leveling` VALUES (2, 53407, 22);
INSERT INTO `learning_spell_leveling` VALUES (2, 20271, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 53408, 12);
INSERT INTO `learning_spell_leveling` VALUES (2, 48788, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 80069, 27);
INSERT INTO `learning_spell_leveling` VALUES (2, 48950, 13);
INSERT INTO `learning_spell_leveling` VALUES (2, 20066, 30);
INSERT INTO `learning_spell_leveling` VALUES (2, 54043, 16);
INSERT INTO `learning_spell_leveling` VALUES (2, 31789, 14);
INSERT INTO `learning_spell_leveling` VALUES (2, 25780, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 53601, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 20375, 20);
INSERT INTO `learning_spell_leveling` VALUES (2, 20164, 22);
INSERT INTO `learning_spell_leveling` VALUES (2, 20165, 30);
INSERT INTO `learning_spell_leveling` VALUES (2, 21084, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 31801, 60);
INSERT INTO `learning_spell_leveling` VALUES (2, 20166, 38);
INSERT INTO `learning_spell_leveling` VALUES (2, 5502, 10);
INSERT INTO `learning_spell_leveling` VALUES (2, 80039, 40);
INSERT INTO `learning_spell_leveling` VALUES (2, 48943, 28);
INSERT INTO `learning_spell_leveling` VALUES (2, 10326, 20);
INSERT INTO `learning_spell_leveling` VALUES (2, 80062, 7);
INSERT INTO `learning_spell_leveling` VALUES (2, 80053, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 80134, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 80177, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 80205, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 49045, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 49071, 10);
INSERT INTO `learning_spell_leveling` VALUES (3, 13161, 10);
INSERT INTO `learning_spell_leveling` VALUES (3, 80128, 5);
INSERT INTO `learning_spell_leveling` VALUES (3, 5118, 10);
INSERT INTO `learning_spell_leveling` VALUES (3, 27044, 10);
INSERT INTO `learning_spell_leveling` VALUES (3, 13163, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 13159, 20);
INSERT INTO `learning_spell_leveling` VALUES (3, 19263, 8);
INSERT INTO `learning_spell_leveling` VALUES (3, 1462, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 883, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 62757, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 80163, 30);
INSERT INTO `learning_spell_leveling` VALUES (3, 5116, 13);
INSERT INTO `learning_spell_leveling` VALUES (3, 80199, 18);
INSERT INTO `learning_spell_leveling` VALUES (3, 781, 4);
INSERT INTO `learning_spell_leveling` VALUES (3, 2641, 5);
INSERT INTO `learning_spell_leveling` VALUES (3, 20736, 23);
INSERT INTO `learning_spell_leveling` VALUES (3, 6197, 10);
INSERT INTO `learning_spell_leveling` VALUES (3, 80161, 9);
INSERT INTO `learning_spell_leveling` VALUES (3, 80138, 18);
INSERT INTO `learning_spell_leveling` VALUES (3, 1002, 29);
INSERT INTO `learning_spell_leveling` VALUES (3, 6991, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 5384, 6);
INSERT INTO `learning_spell_leveling` VALUES (3, 1543, 19);
INSERT INTO `learning_spell_leveling` VALUES (3, 80140, 20);
INSERT INTO `learning_spell_leveling` VALUES (3, 80139, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 13809, 21);
INSERT INTO `learning_spell_leveling` VALUES (3, 53338, 7);
INSERT INTO `learning_spell_leveling` VALUES (3, 49056, 16);
INSERT INTO `learning_spell_leveling` VALUES (3, 80141, 10);
INSERT INTO `learning_spell_leveling` VALUES (3, 61006, 42);
INSERT INTO `learning_spell_leveling` VALUES (3, 53271, 28);
INSERT INTO `learning_spell_leveling` VALUES (3, 48990, 5);
INSERT INTO `learning_spell_leveling` VALUES (3, 34477, 27);
INSERT INTO `learning_spell_leveling` VALUES (3, 49048, 13);
INSERT INTO `learning_spell_leveling` VALUES (3, 48996, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 982, 5);
INSERT INTO `learning_spell_leveling` VALUES (3, 14327, 39);
INSERT INTO `learning_spell_leveling` VALUES (3, 80153, 22);
INSERT INTO `learning_spell_leveling` VALUES (3, 80157, 3);
INSERT INTO `learning_spell_leveling` VALUES (3, 34490, 30);
INSERT INTO `learning_spell_leveling` VALUES (3, 34600, 60);
INSERT INTO `learning_spell_leveling` VALUES (3, 49052, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 80162, 20);
INSERT INTO `learning_spell_leveling` VALUES (3, 1515, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 1494, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 19878, 32);
INSERT INTO `learning_spell_leveling` VALUES (3, 19879, 50);
INSERT INTO `learning_spell_leveling` VALUES (3, 19880, 26);
INSERT INTO `learning_spell_leveling` VALUES (3, 19882, 40);
INSERT INTO `learning_spell_leveling` VALUES (3, 19885, 24);
INSERT INTO `learning_spell_leveling` VALUES (3, 19883, 10);
INSERT INTO `learning_spell_leveling` VALUES (3, 19884, 10);
INSERT INTO `learning_spell_leveling` VALUES (3, 19801, 37);
INSERT INTO `learning_spell_leveling` VALUES (3, 2974, 3);
INSERT INTO `learning_spell_leveling` VALUES (3, 80155, 40);
INSERT INTO `learning_spell_leveling` VALUES (1, 264, 1);
INSERT INTO `learning_spell_leveling` VALUES (4, 264, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 264, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 5011, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 5011, 1);
INSERT INTO `learning_spell_leveling` VALUES (4, 5011, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 266, 1);
INSERT INTO `learning_spell_leveling` VALUES (4, 266, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 266, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 15590, 1);
INSERT INTO `learning_spell_leveling` VALUES (7, 15590, 1);
INSERT INTO `learning_spell_leveling` VALUES (4, 15590, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 15590, 1);
INSERT INTO `learning_spell_leveling` VALUES (11, 15590, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 227, 1);
INSERT INTO `learning_spell_leveling` VALUES (9, 227, 1);
INSERT INTO `learning_spell_leveling` VALUES (7, 227, 1);
INSERT INTO `learning_spell_leveling` VALUES (5, 227, 1);
INSERT INTO `learning_spell_leveling` VALUES (8, 227, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 227, 1);
INSERT INTO `learning_spell_leveling` VALUES (11, 227, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 1180, 1);
INSERT INTO `learning_spell_leveling` VALUES (9, 1180, 1);
INSERT INTO `learning_spell_leveling` VALUES (7, 1180, 1);
INSERT INTO `learning_spell_leveling` VALUES (4, 1180, 1);
INSERT INTO `learning_spell_leveling` VALUES (5, 1180, 1);
INSERT INTO `learning_spell_leveling` VALUES (8, 1180, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 1180, 1);
INSERT INTO `learning_spell_leveling` VALUES (11, 1180, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 750, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 750, 1);
INSERT INTO `learning_spell_leveling` VALUES (6, 750, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 202, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 202, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 202, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 202, 1);
INSERT INTO `learning_spell_leveling` VALUES (6, 202, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 8737, 1);
INSERT INTO `learning_spell_leveling` VALUES (7, 8737, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 8737, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 8737, 1);
INSERT INTO `learning_spell_leveling` VALUES (6, 8737, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 197, 1);
INSERT INTO `learning_spell_leveling` VALUES (7, 197, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 197, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 197, 1);
INSERT INTO `learning_spell_leveling` VALUES (6, 197, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 3127, 1);
INSERT INTO `learning_spell_leveling` VALUES (4, 3127, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 3127, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 3127, 1);
INSERT INTO `learning_spell_leveling` VALUES (6, 3127, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 196, 1);
INSERT INTO `learning_spell_leveling` VALUES (7, 196, 1);
INSERT INTO `learning_spell_leveling` VALUES (4, 196, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 196, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 196, 1);
INSERT INTO `learning_spell_leveling` VALUES (6, 196, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 201, 1);
INSERT INTO `learning_spell_leveling` VALUES (9, 201, 1);
INSERT INTO `learning_spell_leveling` VALUES (4, 201, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 201, 1);
INSERT INTO `learning_spell_leveling` VALUES (8, 201, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 201, 1);
INSERT INTO `learning_spell_leveling` VALUES (6, 201, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (7, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (11, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (6, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 198, 1);
INSERT INTO `learning_spell_leveling` VALUES (7, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (4, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (5, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (11, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (6, 199, 1);
INSERT INTO `learning_spell_leveling` VALUES (1, 200, 1);
INSERT INTO `learning_spell_leveling` VALUES (2, 200, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 200, 1);
INSERT INTO `learning_spell_leveling` VALUES (11, 200, 1);
INSERT INTO `learning_spell_leveling` VALUES (6, 200, 1);
INSERT INTO `learning_spell_leveling` VALUES (3, 674, 10);
INSERT INTO `learning_spell_leveling` VALUES (4, 674, 10);
INSERT INTO `learning_spell_leveling` VALUES (6, 674, 10);
INSERT INTO `learning_spell_leveling` VALUES (7, 674, 10);

SET FOREIGN_KEY_CHECKS = 1;