#
# Copyright (C) 2009-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/ARBOR
	NAME:=Arbor shield board
	PACKAGES:=kmod-usb-core kmod-usb-ohci kmod-usb2
endef

define Profile/ARBOR/Description
	Package set optimized for the Arbor IO Linux Shield.
endef

$(eval $(call Profile,ARBOR))
