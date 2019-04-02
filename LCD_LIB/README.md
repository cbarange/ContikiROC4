# Utilisation du LCD

Repo des sources (toutes) : https://github.com/lab11/cc2538-base


Repo des sources (exemple lcd) : https://github.com/lab11/cc2538-base/tree/master/foundation-firmware/1.0.1.0/bsp/srf06eb_cc2538/examples/lcd


Pour utiliser le lcd dans un environnement contiki, modifiez la ligne `64 - MODULES += core/sys core/dev core/lib` du Makefile.include situé à la racine de contiki. Y ajouter le chemin d'accès vers les fichiers liés à l'utilisation de l'écran situé dans le répertoire `lcd`. Compilez ensuite avec la commande classique `make TARGET=cc2538dk all`.


La documentation pour utiliser l'ecran se trouve dans le document pdf `srf06eb_bsp_users_guide_swru327.pdf`.
