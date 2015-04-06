$(document).ready(function() {
	var ticket = $('input[name=ticket]').val();
	var beginBtn = $('#beginBtn');
	var judgmentBtn = $('#judgmentBtn');
	var nextBtn = $('#nextBtn');

	if (ticket == '') {
		var settings = makeSettings(this, 'auth', ['user', 'pass']);
		ajax(settings, function(map) {
			if (map.ticket !== undefined) {
				$('input[name=ticket]').val(map.ticket);
				init();
			}
		});
	} else {
		init();
	}
	beginBtn.click(function() {
		begin();
	});
	$('#answers input').keyup(function(e) {
		var val = $(this).val();

		if (0 == val.length) {
			judgmentBtn.button('disable');
		} else {
			judgmentBtn.button('enable');
		}
		if (13 == e.which) {
			judgmentBtn.click();
			judgmentBtn.focus();
		}
	});
	$('input[name=index]').change(function() {
		showQuestion();
	});
	judgmentBtn.click(function() {
		judge(this);
	});
	nextBtn.click(function() {
		var indexText = $('input[name=index]');
		var index = parseInt(indexText.val()) + 1;
		var max = $('[name=numOfQuestion]').val();

//$('#judgment').text('index:' + index + '/' + max);
		judgmentBtn.hideBalloon();
		if (max < index) {
			$(this).attr('disabled', 'disabled');
			$('#judgment').html('<span>End.</span>');
			$('select[name=catID]').selectmenu('enable');
			$('select[name=numOfQuestion]').selectmenu('enable');
			beginBtn.removeAttr('disabled');
			return;
		}
		indexText.val(index);
		indexText.slider('refresh');
		showQuestion();
	});
});
function showProgress() {
	var index = parseInt($('input[name=index]').val());
	var rights = $('#progress td.rightAnswer').length;
	var ratio = rights * 100 / index;

	$('input[name=ratio]').val(ratio + '%')
}
/**
 * 判定結果を表示.
 */
function judge(obj) {
	var num = $('#answers :radio,:checkbox').length;
	var chk = $('#answers :checked');
	var description = $('#answers input').val();

	if (0 < num) {
		// 選択式回答
		if (chk.length != 1) {
			judgment('Please select only one answer.');
			return;
		}
	} else {
		// 入力式回答
		if (description.length == 0) {
			judgment('Please input the answer.');
			return;
		}
	}
	var seq = chk.attr('seq');
	$('input[name=seq]').val(seq);
	var settings = makeSettings(obj, 'judge', ['ticket', 'qID', 'seq', 'description']);
	ajax(settings, function(map) {
		var index = parseInt($('input[name=index]').val()) - 1;
		var td = $('#progress td').eq(index);

		judgment(map.result);
		if (td.hasClass('done')) {
			return;
		}
		td.removeClass('rightAnswer');
		td.removeClass('incorrect');
		if (map.judge) {
			td.addClass('rightAnswer');
		} else {
			td.addClass('incorrect');
			if (num == 0) {
				// 入力式回答
				$('#judgmentBtn').showBalloon({
					position: 'right',
					contents: map.answer,
					hideDuration: 0
				});
			}
		}
		td.addClass('done');
		showProgress();
	});
}
function judgment(msg) {
	var judgment = $('#judgment');

	judgment.hide();
	judgment.text(msg);
	judgment.show();
}
function convertQuestion(question) {
	var result = question;
	var words = ['誤っている', '適切でない'];
	$(words).each(function(ix, word) {
		result = result.replace(word, '<span class="mistake">' + word + '</span>');
	});
	return result;
}
/**
 * 質問と選択肢を生成.
 */
function showQuestion() {
	var beginBtn = $('#beginBtn');
	var list = beginBtn.data('list');
	var index = $('input[name=index]').val() - 1;
	var qID = list[index];

	$('input[name=qID]').val(list[index]);
	var settings = makeSettings(beginBtn, 'next', ['ticket', 'catID', 'qID']);
	ajax(settings, function(map) {
		var question = convertQuestion(map['question']);
		var questionText = $('#questionText');
		var fieldset = $('#answers fieldset');
		var inp = $('#answers input');
		var judgmentBtn = $('#judgmentBtn');

		$('input[name=qID]').val(map['qID']);
		if (questionText.hasClass('ui-resizable')) {
			questionText.resizable('destroy');
		}
		questionText.html(question);
//		questionText.resizable();
		questionText.show();
		fieldset.empty();
		if (1 < map.answer.length) {
			// 選択式回答
			var isRadio = true;
			$(map.answer).each(function(iy, rec) {
				var radio = $('<input type="radio"/>');
				var label = $('<label>' + rec.content + '</label>');

				radio.attr('id', rec.seq);
				radio.attr('seq', rec.seq);
				if (isRadio) {
					radio.attr('name', 'answer');
				}
				label.attr('for', rec.seq);
				fieldset.append(radio);
				fieldset.append(label);
			});
			var controls = fieldset.find('input');
			controls.checkboxradio();
			controls.change(function() {
				judgmentBtn.removeAttr('disabled');
			});
			fieldset.show();
			inp.hide();
		} else {
			// 入力式回答
			inp.val('');
			inp.show();
			inp.focus();
			fieldset.hide();
		}
		judgmentBtn.attr('disabled', true);
		judgmentBtn.show();
		$('#judgment').empty();
	});
}
/**
 * 開始.
 */
function begin() {
	var beginBtn = $('#beginBtn');
	var max = parseInt($('[name=numOfQuestion]').val());
	var progress = $('#progress');
	var indexText = $('input[name=index]');

	progress.empty();
	for (var cnt = 1; cnt <= max; cnt++) {
		if ((cnt - 1) % 50 == 0) {
			tr = $('<tr></tr>')
			progress.append(tr);
		}
		var td = $('<td class="numeric">' + cnt + '</td>')
		tr.append(td);
	}
	$('select[name=catID]').selectmenu('disable');
	$('select[name=numOfQuestion]').selectmenu('disable');
	beginBtn.attr('disabled', true);
	indexText.attr('max', max);
	indexText.val(1);
	indexText.slider('refresh');
	$('input[name=rights]').val(0);
	showProgress();
	//
	var settings = makeSettings(beginBtn, 'begin', ['ticket', 'catID', 'numOfQuestion']);
	ajax(settings, function(map) {
		beginBtn.data('list', map.list);
		$('#nextBtn').removeAttr('disabled');
		$('#game').show();
		showQuestion();
	});
}
function init() {
	var select = $('select[name=catID]');
	var settings = makeSettings(select, 'list', ['ticket']);

	ajax(settings, function(map) {
		$(map['list']).each(function(iy, rec) {
			var text = rec.catName + ' (' + rec.cnt + ')';
			var opt = $('<option>' + text + '</option>');
			opt.attr('value', rec.catID);
			opt.attr('cnt', rec.cnt);
			select.append(opt);
		});
		
		select.change(function() {
			categoryChanged(select);
		});
		select.trigger('change');
	});
}
function categoryChanged(select) {
	var opt = select.find('option:selected');
	var max = opt.attr('cnt');
	var select = $('select[name=numOfQuestion]');

	select.empty();
	$([10, 20, 50, 100]).each(function(ix, val) {
		if (max < val) {
			return false;
		}
		var opt = $('<option>' + val + '</option>');
		opt.attr('value', val);
		select.append(opt);
	});
	select.trigger('change');
}
function makeSettings(base, act, nameList) {
	var tr = $(base).parents('form:first');
	var data = {act: act};
	$(nameList).each(function(ix, name) {
		var isCheckbox = 0 < tr.find('[name=' + name + ']:checkbox').length;
		var isSelect = 0 < tr.find('[name=' + name + ']').find('option:selected').length;
		var elm = tr.find('[name=' + name + ']');
		var val = elm.val();
		if (isCheckbox) {
			data[name] = elm.get(0).checked;
			return;
		}
		if (isSelect) {
			data[name] = elm.find('option:selected').attr('value');
			return;
		}
		if (typeof val !== 'undefined') {
			data[name] = val;
			return;
		}
		var selector = '#mainForm input[name=' + name + ']';
		data[name] = $(selector).val();
	});
	return {type:'POST', url:'./app/', data: data};
}
function ajax(settings, func) {
	$.ajax(settings).done(function(values) {
		$('#resultText').val(JSON.stringify(values, null, 2));
		if (values.cause !== undefined) {
			alert(values.cause);
			return;
		}
		func(values);
	});
}
