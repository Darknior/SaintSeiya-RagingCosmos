#!/usr/bin/env python3
from pathlib import Path
import json
R=Path(__file__).resolve().parents[1]/'translations'
CODES=['en','fr','es','pt','zh','ru','ja'];NAMES=['English','Français','Español','Português','中文','Русский','日本語']
KEYS='language select_hint press_start change_language pause continue levels howto quit choose_hint resume_hint levels_hint restart_hint guide_footer controls devices guide_title stage page temple road guards boss'.split()
LABELS=[
'Language|Directions: choose • Attack / Start: confirm|PRESS START|Block: language|PAUSE|Continue|Level Select|How to Play|End Game|Up / Down: choose • Attack / Start: confirm|Jump / Esc: resume|Attack / Start: load • Jump / Esc: back|Keeps your character; restarts the selected stage.|Directions: page • Attack: next • Jump / Start: back|Buttons: Options / Control Options.|Keyboard, controller or touch controls.|HOW TO PLAY — SEIYA|Stage|Page|Temple|Road|Guards|Boss',
'Langue|Directions : choisir • Attaque / Start : valider|APPUYEZ SUR START|Block : langue|PAUSE|Continuer|Choix du niveau|Comment jouer|Quitter la partie|Haut / Bas : choisir • Attaque / Start : valider|Saut / Échap : reprendre|Attaque / Start : charger • Saut / Échap : retour|Garde le personnage et recommence le niveau choisi.|Directions : page • Attaque : suite • Saut / Start : retour|Touches : Options / Control Options.|Clavier, manette ou commandes tactiles.|COMMENT JOUER — SEIYA|Niveau|Page|Temple|Chemin|Gardes|Boss',
'Idioma|Direcciones: elegir • Ataque / Start: confirmar|PULSA START|Block: idioma|PAUSA|Continuar|Elegir nivel|Cómo jugar|Salir de la partida|Arriba / Abajo: elegir • Ataque / Start: confirmar|Salto / Esc: continuar|Ataque / Start: cargar • Salto / Esc: volver|Conserva tu personaje y reinicia el nivel elegido.|Direcciones: página • Ataque: siguiente • Salto / Start: volver|Botones: Options / Control Options.|Teclado, mando o controles táctiles.|CÓMO JUGAR — SEIYA|Nivel|Página|Templo|Camino|Guardias|Jefe',
'Idioma|Direções: escolher • Ataque / Start: confirmar|PRESSIONE START|Block: idioma|PAUSA|Continuar|Selecionar fase|Como jogar|Encerrar partida|Cima / Baixo: escolher • Ataque / Start: confirmar|Pulo / Esc: continuar|Ataque / Start: carregar • Pulo / Esc: voltar|Mantém o personagem e reinicia a fase escolhida.|Direções: página • Ataque: próxima • Pulo / Start: voltar|Botões: Options / Control Options.|Teclado, controle ou comandos de toque.|COMO JOGAR — SEIYA|Fase|Página|Templo|Caminho|Guardas|Chefe',
'语言|方向键：选择 • 攻击 / Start：确认|按 START 开始|Block：切换语言|暂停|继续游戏|选择关卡|操作说明|结束游戏|上 / 下：选择 • 攻击 / Start：确认|跳跃 / Esc：继续|攻击 / Start：载入 • 跳跃 / Esc：返回|保留角色，并重新开始所选关卡。|方向键：翻页 • 攻击：下一页 • 跳跃 / Start：返回|按键设置：Options / Control Options。|支持键盘、手柄和触屏操作。|操作说明 — 星矢|关卡|页|神殿|道路|守卫|首领',
'Язык|Направления: выбор • Атака / Start: подтвердить|НАЖМИТЕ START|Block: язык|ПАУЗА|Продолжить|Выбор уровня|Как играть|Завершить игру|Вверх / Вниз: выбор • Атака / Start: подтвердить|Прыжок / Esc: продолжить|Атака / Start: загрузить • Прыжок / Esc: назад|Персонаж сохранится, выбранный уровень начнётся заново.|Направления: страница • Атака: далее • Прыжок / Start: назад|Кнопки: Options / Control Options.|Клавиатура, геймпад или сенсорное управление.|КАК ИГРАТЬ — СЕЙЯ|Уровень|Страница|Храм|Путь|Стража|Босс',
'言語|方向：選択 • 攻撃 / Start：決定|START を押してください|Block：言語を変更|ポーズ|ゲームに戻る|ステージ選択|遊び方|ゲームを終了|上 / 下：選択 • 攻撃 / Start：決定|ジャンプ / Esc：ゲームに戻る|攻撃 / Start：開始 • ジャンプ / Esc：戻る|キャラクターを引き継いで、選んだステージを最初から開始。|方向：ページ • 攻撃：次へ • ジャンプ / Start：戻る|ボタン設定：Options / Control Options。|キーボード・ゲームパッド・タッチ操作に対応。|遊び方 — 星矢|ステージ|ページ|神殿|道中|兵士|ボス']
PAGES=[
[
'Move|DIRECTIONS|Up / Down / Left / Right|Line Seiya up with his opponent.|Attacks connect on the same lane.',
'Run|TAP THE SAME DIRECTION TWICE|Double-tap Left or Right.|Hold the direction to keep running.|Press Attack to strike while running.',
'Chain attacks|PRESS ATTACK REPEATEDLY|Press Attack several times.|Seiya chains five strikes.|Get close enough to hit the target.',
'Jump|JUMP|Press Jump.|Add a direction to jump forward.|Watch out for gaps in the stage!',
'Attack in the air|JUMP, THEN ATTACK|Jump, then press Attack.|Seiya strikes during his jump.|Try it while jumping forward too.',
'Block|HOLD BLOCK|Hold the button named Block.|Face the incoming attacks.|Release it to resume fighting.',
'Recharge Cosmos|HOLD CHARGE|Hold the button named Charge.|The Cosmos gauge fills up.|Find a safe spot away from attacks.'],
[
'Se déplacer|DIRECTIONS|Haut / Bas / Gauche / Droite|Aligne Seiya avec son adversaire.|Les coups portent sur la même ligne.',
'Courir|DEUX FOIS LA MÊME DIRECTION|Appuie deux fois sur Gauche ou Droite.|Garde la direction pour continuer.|Attaque en courant : Attaque.',
'Enchaîner les coups|ATTAQUE — APPUIS SUCCESSIFS|Appuie plusieurs fois sur Attaque.|Seiya enchaîne cinq coups.|Approche-toi pour toucher la cible.',
'Sauter|SAUT|Appuie sur Saut.|Ajoute une direction pour avancer.|Attention aux trous du décor !',
'Attaquer en l’air|SAUT PUIS ATTAQUE|Saute, puis appuie sur Attaque.|Seiya frappe pendant son saut.|Essaie aussi en sautant vers l’avant.',
'Se protéger|MAINTENIR BLOCK|Maintiens le bouton nommé Block.|Tourne-toi vers les coups à bloquer.|Relâche pour reprendre le combat.',
'Recharger le Cosmos|MAINTENIR CHARGE|Maintiens le bouton nommé Charge.|La jauge Cosmos se remplit.|Choisis un endroit à l’abri des coups.'],
[
'Moverse|DIRECCIONES|Arriba / Abajo / Izquierda / Derecha|Alinea a Seiya con su adversario.|Los golpes conectan en la misma línea.',
'Correr|DOS VECES LA MISMA DIRECCIÓN|Pulsa dos veces Izquierda o Derecha.|Mantén la dirección para seguir.|Pulsa Ataque para golpear al correr.',
'Encadenar golpes|PULSA ATAQUE VARIAS VECES|Pulsa Ataque repetidamente.|Seiya encadena cinco golpes.|Acércate para alcanzar al enemigo.',
'Saltar|SALTO|Pulsa Salto.|Añade una dirección para avanzar.|¡Cuidado con los huecos del escenario!',
'Atacar en el aire|SALTO Y DESPUÉS ATAQUE|Salta y luego pulsa Ataque.|Seiya golpea durante el salto.|Prueba también saltando hacia delante.',
'Protegerse|MANTENER BLOCK|Mantén el botón llamado Block.|Mira hacia los ataques que recibes.|Suéltalo para volver a luchar.',
'Recargar el Cosmos|MANTENER CHARGE|Mantén el botón llamado Charge.|La barra de Cosmos se llena.|Busca un lugar a salvo de los golpes.'],
[
'Mover-se|DIREÇÕES|Cima / Baixo / Esquerda / Direita|Alinhe Seiya com o adversário.|Os golpes acertam na mesma linha.',
'Correr|DUAS VEZES NA MESMA DIREÇÃO|Toque duas vezes em Esquerda ou Direita.|Mantenha a direção para continuar.|Aperte Ataque para golpear correndo.',
'Combinar golpes|APERTE ATAQUE VÁRIAS VEZES|Aperte Ataque repetidamente.|Seiya combina cinco golpes.|Aproxime-se para atingir o inimigo.',
'Pular|PULO|Aperte Pulo.|Use uma direção para avançar.|Cuidado com os buracos do cenário!',
'Atacar no ar|PULO E DEPOIS ATAQUE|Pule e depois aperte Ataque.|Seiya ataca durante o pulo.|Experimente também pulando para a frente.',
'Defender-se|SEGURE BLOCK|Segure o botão chamado Block.|Fique de frente para os ataques.|Solte para voltar a lutar.',
'Recarregar o Cosmos|SEGURE CHARGE|Segure o botão chamado Charge.|A barra de Cosmos é recarregada.|Escolha um lugar protegido dos golpes.'],
[
'移动|方向键|上 / 下 / 左 / 右|让星矢与敌人处于同一直线上。|在同一直线上才能击中敌人。',
'奔跑|连续按两次同一方向|连续按两次左或右。|按住方向键可继续奔跑。|奔跑时按攻击可出招。',
'连续攻击|连续按攻击|多次按攻击键。|星矢会使出五段连击。|靠近敌人后再出拳。',
'跳跃|跳跃|按跳跃键。|配合方向键可向前跳跃。|小心场景中的深坑！',
'空中攻击|先跳跃，再攻击|跳起后按攻击键。|星矢会在空中发动攻击。|也可以在向前跳跃时尝试。',
'防御|按住 BLOCK|按住名为 Block 的按键。|面向敌人的攻击方向。|松开按键即可继续战斗。',
'回复小宇宙|按住 CHARGE|按住名为 Charge 的按键。|小宇宙能量条会逐渐充满。|在不易受到攻击的地方蓄力。'],
[
'Передвижение|НАПРАВЛЕНИЯ|Вверх / Вниз / Влево / Вправо|Встаньте с противником на одну линию.|Удары попадают по целям на той же линии.',
'Бег|ДВАЖДЫ В ОДНОМ НАПРАВЛЕНИИ|Дважды нажмите Влево или Вправо.|Удерживайте направление, чтобы бежать.|Нажмите Атаку для удара на бегу.',
'Серия ударов|НАЖИМАЙТЕ АТАКУ НЕСКОЛЬКО РАЗ|Несколько раз нажмите Атаку.|Сейя выполняет серию из пяти ударов.|Подойдите ближе, чтобы попасть по врагу.',
'Прыжок|ПРЫЖОК|Нажмите Прыжок.|Добавьте направление для прыжка вперёд.|Остерегайтесь пропастей на уровне!',
'Атака в воздухе|ПРЫЖОК, ЗАТЕМ АТАКА|Прыгните, затем нажмите Атаку.|Сейя атакует во время прыжка.|Попробуйте также прыжок вперёд.',
'Защита|УДЕРЖИВАЙТЕ BLOCK|Удерживайте кнопку с названием Block.|Повернитесь лицом к атакующему врагу.|Отпустите кнопку, чтобы снова атаковать.',
'Восполнение Космоса|УДЕРЖИВАЙТЕ CHARGE|Удерживайте кнопку с названием Charge.|Шкала Космоса постепенно заполняется.|Выберите место, защищённое от ударов.'],
[
'移動|方向ボタン|上 / 下 / 左 / 右|星矢と敵を同じラインに合わせよう。|同じラインにいる敵に攻撃が当たる。',
'走る|同じ方向を素早く２回|左か右を素早く２回押す。|そのまま方向を押し続けると走る。|走りながら攻撃を押すとダッシュ攻撃。',
'連続攻撃|攻撃を連続で押す|攻撃ボタンを何度も押そう。|星矢が５段の連続攻撃を繰り出す。|敵に近づいてから攻撃しよう。',
'ジャンプ|ジャンプ|ジャンプボタンを押す。|方向を組み合わせると前に跳べる。|ステージの穴には気をつけよう！',
'空中攻撃|ジャンプしてから攻撃|ジャンプした後に攻撃を押す。|星矢が空中で攻撃する。|前に跳びながらの攻撃も試そう。',
'防御|BLOCK を押し続ける|Block という名前のボタンを押し続ける。|攻撃してくる敵の方を向こう。|ボタンを離すと再び戦える。',
'小宇宙をためる|CHARGE を押し続ける|Charge という名前のボタンを押し続ける。|小宇宙ゲージが回復する。|敵に攻撃されにくい場所でためよう。']]
MAIN=[['Start Game','Options','How to Play','Hall of Fame','Quit'],['Jouer','Options','Comment jouer','Meilleurs scores','Quitter'],['Jugar','Opciones','Cómo jugar','Mejores puntuaciones','Salir'],['Jogar','Opções','Como jogar','Melhores pontuações','Sair'],['开始游戏','设置','操作说明','最高分','退出'],['Начать игру','Настройки','Как играть','Таблица рекордов','Выход'],['ゲーム開始','設定','遊び方','ハイスコア','終了']]
for i,code in enumerate(CODES):
 values=LABELS[i].split('|');assert len(values)==len(KEYS)
 d={'code':code,'name':NAMES[i],'ui':dict(zip(KEYS,values)),'main':MAIN[i],'pages':[dict(zip(['title','command','line1','line2','line3'],v.split('|'))) for v in PAGES[i]],'variant':{'pt':'pt-BR','zh':'zh-Hans'}.get(code,code)}
 (R/(code+'.json')).write_text(json.dumps(d,ensure_ascii=False,indent=2)+'\n')
print('Seven UTF-8 catalogues created')
