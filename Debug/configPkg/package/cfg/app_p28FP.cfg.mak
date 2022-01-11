# invoke SourceDir generated makefile for app.p28FP
app.p28FP: .libraries,app.p28FP
.libraries,app.p28FP: package/cfg/app_p28FP.xdl
	$(MAKE) -f C:\Users\Fabbio\workspace_v10\generator/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Fabbio\workspace_v10\generator/src/makefile.libs clean

