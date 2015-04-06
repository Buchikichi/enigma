<?php
function scanDirectory($dir, &$list) {
	foreach (scandir($dir) as $name) {
		if (strpos($name, '.') === 0) {
			continue;
		}
		$fullname = $dir.'/'.$name;
		if (!is_dir($fullname)) {
			continue;
		}
		$list[] = $fullname;
		scanDirectory($fullname, $list);
	}
}
$list = array();
scanDirectory(APP_ROOT, $list);
spl_autoload_register(function ($class) {
	global $list;
	$classFile = $class.'.inc';

	foreach ($list as $dirname) {
		$filename = $dirname.'/'.$classFile;
		if (is_file($filename)) {
			require_once($filename);
			break;
		}
	}
});
