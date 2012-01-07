function cyy_tab_closeRightTabs()
{
	let dom=gBrowser.tabContainer;
	while(dom.itemCount>dom.selectedIndex+1)
		dom.removeItemAt(dom.selectedIndex+1);
}
