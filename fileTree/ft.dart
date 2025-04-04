import 'dart:io';
import 'dart:async';

bool vediamo = true;
List<String> oldFileTree = [];
List<String> emptySubDir = [];

Future<void> fileTreeLoop() async {
  var result = await Process.run('cmd', ['/c', 'where /r . *']);
  var currentDir = await Process.run('cmd', ['/c', 'cd']);
  var possibleEmptyDir = await Process.run('cmd', ['/c', 'dir /AD /B /S']);
  List<RegExp> regexList = [
    // RegExp(r'.*\.[a-zA-Z0-9]*\/.*'),
    RegExp(r'(^|/)\.[^/]+(/|$)'),
    RegExp(r'.*__.*__.*'),
    RegExp(r'.*build\/?.*'),
    RegExp(r'.*android\/?.*'),
    RegExp(r'.*windows\/?.*'),
    RegExp(r'.*ios\/?.*'),
    RegExp(r'.*macos\/?.*'),
    RegExp(r'.*linux\/?.*'),
    RegExp(r'.*web\/?.*'),
    RegExp(r'.*node_modules\/?.*'),
  ];

  String newCurrentDir =
      currentDir.stdout.toString().trim().replaceAll('\\', '/') + '/';
  var split =
      result.stdout
          .toString()
          .replaceAll('\\', '/')
          .replaceAll(newCurrentDir, '')
          .split('\n')
          .where((string) {
            return !regexList.any((regex) => regex.hasMatch(string));
          })
          .map((e) => e.trim())
          .toList();
  emptySubDir =
      possibleEmptyDir.stdout
          .toString()
          .replaceAll('\\', '/')
          .replaceAll(newCurrentDir, '')
          .split('\n')
          .where((string) {
            return !regexList.any((regex) => regex.hasMatch(string));
          })
          .map((e) => e.trim())
          .toList()
          // emptySubDir =
          // emptySubDir
          .where((element) => !split.any((s) => s.startsWith(element)))
          .map((e) => e + ' <Empty Folder>')
          .toList();
  if (emptySubDir.isNotEmpty) {
    split.addAll(emptySubDir);
  }
  split = orderedList(split);
  if (split.where((element) => !oldFileTree.contains(element)).length > 0 ||
      split.length != oldFileTree.length) {
    stdout.write('\x1B[2J\x1B[H'); //pulisce la pagina del terminale
    stdout.flush(); // Forza l'output immediato
    oldFileTree = split;
    for (var dir in fileTree(split)) {
      //nel caso in cui la stringa contiene solo questi caratteri
      //può essere eliminata
      if (!RegExp(r'^[└─├│ ]*$').hasMatch(dir)) {
        print(dir);
      }
    }
  }

  if (result.stderr.isNotEmpty) {
    print('Errore: ${result.stderr}');
  }
}

List<String> orderedList(List<String> dirs) {
  Map<int, String> dirsMap = Map.from(dirs.asMap());
  List<int> indeces = orderedIndex(dirsMap);
  return indeces.map((indice) => dirs[indice]).toList();
}

List<int> orderedIndex(Map<int, String> dirsMap) {
  List<int> buffer = [];
  //trovo gli elementi che non hanno slash e quindi non sono cartelle ma file a se stanti
  for (var entry in dirsMap.entries) {
    if (!entry.value.contains('/')) {
      buffer.add(entry.key);
    }
  }
  //elimino dalla Map gli elementi trovati precedentemente così da avere solo sotto cartelle
  for (int keyToRemove in buffer) {
    dirsMap.remove(keyToRemove);
  }
  // Se la Map è vuota, interrompo la ricorsione (caso base)
  if (dirsMap.isEmpty) {
    return buffer;
  }
  List<String> uniqueStart = getSubDirsNames(dirsMap.values.toList());
  List<Map<int, String>> listOfMap = List.generate(
    uniqueStart.length,
    (_) => Map(),
  );
  //se il value inizia con uno di uniqueStart, lo metto in listOfMap[uniqueStart.indexOf(i)]
  // e tolgo dal String value il uniqueStart[i]
  for (var entry in dirsMap.entries) {
    for (int i = 0; i < uniqueStart.length; i++) {
      if (entry.value.startsWith(uniqueStart[i])) {
        listOfMap[i][entry.key] = entry.value.replaceFirst(
          uniqueStart[i] + '/',
          '',
        );
        break;
      }
    }
  }
  //Ricorsione
  if (listOfMap.isNotEmpty) {
    for (Map<int, String> newDirsMap in listOfMap) {
      buffer.addAll(orderedIndex(newDirsMap));
    }
  }
  return buffer;
}

void main() async {
  Timer.periodic(Duration(seconds: 1), (timer) {
    fileTreeLoop();
  });
}

List<String> fileTree(List<String> dirs) {
  List<String> buffer = [];

  for (int i = 0; i < dirs.length; i++) {
    if (!dirs[i].contains('/')) {
      buffer.add(dirs[i]);
    } else {
      List<String> nameSubDirs = getSubDirsNames(dirs.sublist(i));

      for (String name in nameSubDirs) {
        // if (emptySubDir.any((e) => e.endsWith(name))) {
        // for (var esd in emptySubDir) {
        //   if (esd.endsWith(name)) {
        //     emptySubDir.remove(esd);
        //     break;
        //   }
        // }
        // buffer.add(name + '/ (Empty Folder)');
        // } else {
        buffer.add(name + '/');
        buffer.addAll(fileTreeRec(getFilesSubDir(dirs.sublist(i), name)));
        // }
      }

      i = dirs.length;
    }
  }
  return buffer;
}

List<String> fileTreeRec(List<String> dirs) {
  List<String> buffer = [];

  int spaces = 2;
  //transforma il dirs in set di elementi che hanno uno start di stringa univoco
  // prima del primo \ e prendi la lunghezza di questo
  List<String> baseSymbolsTree = baseSymbolsTreeSeq(dirs);

  for (int i = 0; i < dirs.length; i++) {
    if (!dirs[i].contains('/')) {
      buffer.add(" " * spaces + baseSymbolsTree[i] + dirs[i]);
    } else {
      List<String> nameSubDirs = getSubDirsNames(dirs.sublist(i));
      List<String> newDirs = dirs.sublist(i);
      List<String> newBaseSymbolsTree = baseSymbolsTree.sublist(i);
      List<String> newBuffer = [];
      for (String name in nameSubDirs) {
        // if (emptySubDir.any((element) => element.endsWith(name))) {
        //   print("dentro");
        //   for (var esd in emptySubDir) {
        //     if (esd.endsWith(name)) {
        //       emptySubDir.remove(esd);
        //       break;
        //     }
        //   }
        // newBuffer.add(name + '/ (Empty Folder)');
        // } else {
        // print("else $name");
        newBuffer.add(name + '/');
        // }
        for (String s in fileTreeRec(getFilesSubDir(newDirs, name))) {
          newBuffer.add(s);
        }
      }
      for (int i = 0; i < newBuffer.length; i++) {
        if (i < newBaseSymbolsTree.length) {
          buffer.add(" " * spaces + newBaseSymbolsTree[i] + newBuffer[i]);
        } else {
          buffer.add(" " * spaces + newBuffer[i]);
        }
      }

      i = dirs.length;
    }
  }
  return buffer;
}

List<String> getSubDirsNames(List<String> dirs) {
  Set<String> setSubDirs = {};
  RegExp regexpNormalDir = RegExp(r'^[^/]*');
  RegExp regexpEmptyDir = RegExp(r'^[^.]*$');
  for (var i = 0; i < dirs.length; i++) {
    if (regexpNormalDir.hasMatch(dirs[i])) {
      setSubDirs.add(regexpNormalDir.stringMatch(dirs[i])!);
    } else if (regexpEmptyDir.hasMatch(dirs[i])) {
      setSubDirs.add(regexpEmptyDir.stringMatch(dirs[i])!);
    }
  }
  return setSubDirs.toList();
}

List<String> getFilesSubDir(List<String> dirs, String nameSub) {
  List<String> buffer = [];

  for (String dir in dirs) {
    if (dir.startsWith(nameSub + '/')) {
      // if (dir.contains(nameSub + '/')) {
      buffer.add(dir.replaceFirst(nameSub + '/', ''));
    }
  }
  return buffer;
}

List<String> baseSymbolsTreeSeq(List<String> dirs) {
  Set<String> uniqueStart = {};
  List<String> symbols = [];
  RegExp regexp = RegExp(r'^[^/]*');
  // questo for crea il set con inizi "unici" delle directory date
  for (var i = 0; i < dirs.length; i++) {
    String s = regexp.stringMatch(dirs[i])!;
    if (!uniqueStart.contains(s)) {
      uniqueStart.add(s);
    }
  }
  // inserisce per ora i sumboli base, dopo inserisco i simboli di conginuzione
  for (int i = 0; i < uniqueStart.length - 1; i++) {
    symbols.add('├─');
  }
  symbols.add('└─');
  //List di List, ha una lista per ogni unique start meno 1, in ogni lista verranno messi i simboli di congiunzione
  //relativi allo stesso index di uniqueStart
  List<List<String>> symbolsOfCong = List.generate(
    uniqueStart.length > 0 ? uniqueStart.length - 1 : 0,
    (_) => [],
  );
  Set<String> uniqueSubDir = {};
  // Inserisce i simboli di congiunzione
  for (int i = 0; i < uniqueStart.length - 1; i++) {
    for (String dir in dirs) {
      int numOfSubDir = getNumOfSubDir(dir);
      String startStr = regexp.stringMatch(dir)!;
      if (uniqueStart.elementAt(i) == startStr && numOfSubDir != 0) {
        if (numOfSubDir > 1) {
          List<String> newSubDirsUnique = dir.split("/");
          newSubDirsUnique.removeAt(0);
          newSubDirsUnique.removeLast();
          for (String newSub in newSubDirsUnique) {
            if (uniqueSubDir.contains(newSub)) {
              numOfSubDir--;
            } else {
              uniqueSubDir.add(newSub);
            }
          }
        }
        for (int j = 0; j < numOfSubDir; j++) {
          symbolsOfCong[i].add('│');
        }
      }
    }
  }
  //Crea lista finale unendo i simboli per ogni dir unica e di congiunzione
  List<String> finalSymbols = [];
  for (int i = 0; i < symbolsOfCong.length; i++) {
    finalSymbols.add(symbols[i]);
    finalSymbols.addAll(symbolsOfCong[i]);
  }
  finalSymbols.add(symbols.last);
  return finalSymbols;
}

int getNumOfSubDir(String dir) {
  return dir.split('/').length - 1;
}
