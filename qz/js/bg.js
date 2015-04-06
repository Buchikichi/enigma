/**
 * 
 */
$(document).ready(function() {
	$('#tabs').tabs();
	// [Play]
	$('#beginBtn').click(function() {
		var settings = makeSettings(this, 'begin', ['ticket', 'catID', 'numOfQuestion']);
		showResult(settings, function(map) {
			makeQuestion(map);
		});
	});
	$('#answers ol').selectable({
		stop: function() {
			var li = $('#answers li.ui-selected');

			if (0 == li.length) {
				$('#judgmentBtn').attr('disabled', 'disabled');
			} else {
				$('#judgmentBtn').removeAttr('disabled');
			}
		}
	});
	$('#judgmentBtn').click(function() {
		judge();
	});
	$('#nextBtn').click(function() {
		var settings = makeSettings(this, 'next', ['ticket', 'catID']);
		showResult(settings, function(map) {
			makeQuestion(map);
		});
	});
	// [Category]
	$('#catCreateBtn').click(function() {
	});
	$('#catListBtn').click(function() {
		var settings = makeSettings(this, 'list', ['ticket']);
		showResult(settings, function(map) {
			$('#resultTable tbody tr').click(function() {
				var tr = $(this);
				var catID = tr.find('td:eq(0)').text();
				var catName = tr.find('td:eq(1)').text();
				$('input[name=catID]').val(catID);
				$('#catName').text(catName);
			});
		});
	});
	$('#questionUpdateBtn').click(function() {
		var settings = makeSettings(this, 'qup', ['ticket', 'catID']);
		showResult(settings, function(map) {
		});
	});
	$('#authenticateBtn').click(function() {
		var settings = makeSettings(this, 'auth', ['user', 'pass']);
		showResult(settings, function(map) {
			$('input[name=ticket]').val(map['ticket']);
		});
	});
	// [▼]ボタン
	$('#resultIcon').click(function() {
		var btn = $(this);
		if (btn.hasClass('ui-icon-triangle-1-e')) {
			btn.removeClass('ui-icon-triangle-1-e');
			btn.addClass('ui-icon-triangle-1-s');
		} else {
			btn.removeClass('ui-icon-triangle-1-s');
			btn.addClass('ui-icon-triangle-1-e');
		}
		$('#resultText').toggle();
	});
});
function makeSettings(base, act, nameList) {
	var tr = $(base).parents('tr:first');
	var data = {act: act};
	$(nameList).each(function(ix, name) {
		var isCheckbox = 0 < tr.find('[name=' + name + ']:checkbox').length;
		var elm = tr.find('[name=' + name + ']');
		var val = elm.val();
		if (isCheckbox) {
			data[name] = elm.get(0).checked;
			return;
		} else if (typeof val !== 'undefined') {
			data[name] = val;
			return;
		}
		var selector = '#mainForm input[name=' + name + ']';
		data[name] = $(selector).val();
	});
	return {type:'POST', url:'./app/', data: data};
}
function showResult(settings, func) {
	$.ajax(settings).done(function(values) {
		hideResult();
		$('#resultText').val(JSON.stringify(values, null, 2));
		if (values.list !== undefined) {
			var fields = ['catID', 'catName', 'cnt'];
			makeResultTable(values, fields);
		}
		if (func) {
			func(values);
		}
	});
}
function hideResult() {
	$('#resultTable').hide();
	$('#resultText').hide();
}
/**
 * 結果一覧テーブルを作成する.
 * @param map サーバーから取得した情報
 * @param fields フィールド情報
 */
function makeResultTable(map, fields) {
	var resultTable = $('#resultTable');
	var tbody = $('<tbody></tbody>');

	resultTable.empty();
	var caption = $('<caption>' + map.list.length + ' of ' + map.count + '</caption>');
	resultTable.append(caption);
	makeThead(fields);
	$(map.list).each(function(iy, rec) {
		var tr = $('<tr></tr>');
		$(fields).each(function(ix, name) {
			var val = rec[name] ? rec[name] : '';
			var td = $('<td>' + val + '</td>');

			td.addClass(name);
			tr.append(td);
		});
		tbody.append(tr);
	});
	resultTable.append(tbody);
	resultTable.show();
	return resultTable;
}
/**
 * thead部を作成する.
 * @param fields
 */
function makeThead(fields) {
	var resultTable = $('#resultTable');
	var thead = $('<thead></thead>');
	var tr = $('<tr></tr>');

	thead.append(tr);
	resultTable.append(thead);
	$(fields).each(function(ix, name) {
		var th = $('<th>' + name + '</th>');
		tr.append(th);
	});
}
/**
 * 質問と選択肢を生成.
 * @param map
 */
function makeQuestion(map) {
	var question = convertQuestion(map['question']);
	var questionText = $('#questionText');
	var ol = $('#answers ol');

	$('input[name=qID]').val(map['qID']);
	if (questionText.hasClass('ui-resizable')) {
		questionText.resizable('destroy');
	}
	questionText.html(question);
	questionText.resizable();
	questionText.show();
	ol.empty();
	$(map.answer).each(function(iy, rec) {
		var li = $('<li>' + rec.content + '</li>');
		li.attr('seq', rec.seq);
		ol.append(li);
	});
	ol.selectable('refresh');
	$('#judgmentBtn').attr('disabled', 'disabled');
	$('#judgmentBtn').show();
	$('#judgment').empty();
}
function convertQuestion(question) {
	var result = question.replace('誤っている', '<span class="mistake">誤っている</span>');
	return result;
}
/**
 * 判定結果を表示.
 */
function judge() {
	var li = $('#answers li.ui-selected');
	var judgment = $('#judgment');

	if (li.length != 1) {
		judgment.text('Please select only one answer.');
		return;
	}
	var seq = li.attr('seq');
	$('input[name=seq]').val(seq);
	var settings = makeSettings(this, 'judge', ['ticket', 'qID', 'seq']);
	showResult(settings, function(map) {
		var msg = map.result;
		judgment.text(msg);
	});
	$('#judgmentBtn').hide();
}
