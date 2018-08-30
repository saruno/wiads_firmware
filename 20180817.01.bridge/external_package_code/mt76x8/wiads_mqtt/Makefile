#
# Copyright (C) 2006-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wiads_mqtt
PKG_VERSION:=1.0.0
PKG_RELEASE:=1
PKG_MAINTAINER:=thinhnv
PKG_LICENSE:=CC0-1.0
PKG_CONFIG_DEPENDS:=+libmosquitto-nossl

include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

TARGET_LDFLAGS+= \
  -Wl,-rpath-link=$(STAGING_DIR)/usr/lib \
  -Wl,-rpath-link=$(STAGING_DIR)/usr/lib/mosquitto/lib

define Package/wiads_mqtt
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libmosquitto-nossl
  TITLE:=Wiads Router MQTT
  URL:=http://enter.wiads.vn
endef

define Package/wiads_mqtt/description
 Wiads Router MQTT
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS)
endef

define Package/wiads_mqtt/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wiads_mqtt $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) files/wiads_mqtt.init $(1)/etc/init.d/wiads_mqtt
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) files/wiads_mqtt.config $(1)/etc/config/wiads_mqtt
endef

$(eval $(call BuildPackage,wiads_mqtt))

