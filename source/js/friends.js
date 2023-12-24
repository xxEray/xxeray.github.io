// 随机排列
function shuffle(arr) {
  let i = arr.length;
  while (i) {
	let j = Math.floor(Math.random() * i--);
	[arr[j], arr[i]] = [arr[i], arr[j]];
  }
}

// 渲染数据
function renderfriend(data) {
  var name, avatar, site, li = "";
  shuffle(data);
  for (var i = 0; i < data.length; i++) {
	name = data[i].name;
	avatar = data[i].avatar;
	site = data[i].site;
	li += '<div class="card">' + '<a href="' + site + '" target="_blank">' + '<div class="thumb" style="background: url( ' + avatar + ');">' + '</div>' + '</a>' + '<div class="card-header">' + '<div><a href="' + site + '" target="_blank">' + name + '</a></div>' + '</div>' + '</div>';
  }
  document.querySelector(".friend-navigation").innerHTML = li;
}

// 获取 json 文件
fetch('/friends/list.json')
  .then(response => response.json())
  .then(res => renderfriend(res));
  