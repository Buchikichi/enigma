<?php
define('LOG_PATH', APP_ROOT.'/log/');

/**
 * ログ出力を行うためのクラス.
 * @author ponta
 */
class Logger {
	private static $me = null;

	private function __construct() {
	}

	public static function getInstance() {
		if (is_null(self::$me)) {
			self::$me = new self;
		}
		return self::$me;
	}
	private function write($param) {
		$filename = LOG_PATH.'request'.date('Ymd').'.log';
		$str = date('[Y-m-d H:i:s]');
		if (isset($_SERVER["REMOTE_ADDR"])) {
			$str .= ' '.$_SERVER["REMOTE_ADDR"];
			$str .= ' '.$_SERVER["HTTP_USER_AGENT"]."\n";
		} else {
			$str .= " console\n";
		}
		if (is_array($param)) {
			$opt = JSON_UNESCAPED_UNICODE; // JSON_PRETTY_PRINT|JSON_UNESCAPED_SLASHES
			$str .= '  '.json_encode($param, $opt)."\n";
		} else {
			$str .= '  '.$param."\n";
		}
		file_put_contents($filename, $str, FILE_APPEND);
	}
	public function debug($param) {
		$this->write($param);
	}
}
?>
