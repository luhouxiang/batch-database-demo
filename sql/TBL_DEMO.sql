/*
Navicat SQL Server Data Transfer

Source Server         : 192.168.5.216
Source Server Version : 105000
Source Host           : 192.168.5.216:1433
Source Database       : BATCH_DATABASE_DEMO
Source Schema         : dbo

Target Server Type    : SQL Server
Target Server Version : 105000
File Encoding         : 65001

Date: 2018-01-10 16:55:52
*/


-- ----------------------------
-- Table structure for TBL_DEMO
-- ----------------------------
DROP TABLE [dbo].[TBL_DEMO]
GO
CREATE TABLE [dbo].[TBL_DEMO] (
[ID]  int NOT NULL IDENTITY(1,1) NOT FOR REPLICATION ,
[CODE] varchar(64) NULL ,
[NAME] varchar(64) NULL ,
   CONSTRAINT [PK_TBL_DEMO_1] PRIMARY KEY CLUSTERED 
    (
	    [ID] ASC
    )
)


GO

-- ----------------------------
-- Records of TBL_DEMO
-- ----------------------------

-- ----------------------------
-- Indexes structure for table TBL_DEMO
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table TBL_DEMO
-- ----------------------------
ALTER TABLE [dbo].[TBL_DEMO] ADD PRIMARY KEY ([ID])
GO
