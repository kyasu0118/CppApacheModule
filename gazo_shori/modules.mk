mod_gazo_shori.la: mod_gazo_shori.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_gazo_shori.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_gazo_shori.la
