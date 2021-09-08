#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gfx/di.h"
#include "gfx/gfx.h"
#include "utils/update.h"
#include "utils/btn.h"
#include "core/launcher.h"

#include "utils/util.h"
#include "utils/fs_utils.h"

char *type = "*";
char *clip = "";

void lineHandler(char line[])
{
	/* If line is a comment return */
	if (line[0] == '#')
		return;

	/* If line is a message print it */
	if (line[0] == '[')
	{
		memmove(line, line + 1, strlen(line));
		printerCU(line, "", 0);
		return;
	}
	
	/* If line is copy */
	if (line[0] == '<')
	{
		memmove(line, line + 1, strlen(line));
		clip=line;
		return;
	}

	/* If line is paste */
	if (line[0] == '>')
	{
		memmove(line, line + 1, strlen(line));
		copyfile(clip, line);
		return;
	}

	/* If line is rename */
	if (line[0] == '~')
	{
		memmove(line, line + 1, strlen(line));
		f_rename(clip, line);
		return;
	}

	/* If line is a Title print it */
	if (line[0] == '.')
	{
		memmove(line, line + 1, strlen(line));
		printerCU("", line, 0);
		return;
	}

	/* If line is a ArchBit */
	if (line[0] == ':')
	{
		memmove(line, line + 1, strlen(line));
		if (HasArchBit(line))
		{
			printerCU(line, "", 0);
			Killflags(line);
		}
		return;
	}

	/* If line is make folder */
	if (line[0] == '+')
	{
		memmove(line, line + 1, strlen(line));
		f_mkdir(line);
		return;
	}

	/* If line is a type set type for next directory */
	if (line[0] == '*')
	{
		memmove(line, line + 1, strlen(line));
		type = line;
		return;
	}

	printerCU(line,"",2);
	
	/* If line is a directory delete it all*/
	if (line[strlen(line) - 1] == '/')
	{
		line[strlen(line) - 1] = 0;
		deleteall(line, type, line);
		return;
	}
	if (strlen(line) > 0)
	{
		if (line[0] == '/')
		{
			f_unlink(line);
		}
	}
}

void clean_up()
{
	if (!sd_file_exists("fixer.del")){return;}
	
	printerCU("Clean old files", "CleanUP...", 0);

	FIL delet;
	f_open(&delet, "/StarDust/Main.del", FA_READ);
	FILINFO stats;

	f_stat("/StarDust/Main.del", &stats);
	__off_t size = stats.fsize;

	char buff[size];
	f_read(&delet, buff, size, NULL);

	f_close(&delet);

	strrep(buff, "\r", "");
	char *p = strtok(buff, "\n");
	while (p != NULL)
	{
		lineHandler(p);
		p = strtok(NULL, "\n");
	}

	//Pequeña correccion
	f_rename("/Backup/prodinfo.bin", "/prodinfo_sysnand.bin");

	//not pegascape units
	if (fusesB() > 5)
	{
		printerCU("Remove PegaScape Files", "CleanUP...", 0);
		deleteall("/pegascape", "*", "");
		f_unlink("/switch/fakenews-injector.nro");
	}

	//Fix old Emunand transfer
	fix_emu();

	f_unlink("/fixer.del");
	printerCU("", "", 1); //flush print
}

void Update_SDT(){
	//some test verify payload
	if (sd_file_exists("StarDust/flags/ONE.flag"))
	{
		f_unlink("StarDust/flags/ONE.flag");
		launch_payload("payload.bin");
	}
	//update stardust
	bool cancel_auto_chainloading = btn_read() & BTN_VOL_UP;
	if (sd_file_exists("StarDust_update/fixer.del") & !cancel_auto_chainloading)
	{
		moverall("/StarDust_update", "", "*", "Updating");
		printerCU("Clean Update", "", 0);
		deleteall("/StarDust_update", "*", "Clean Update");
		f_rename("/StarDust_update", "/StarDust_corrupt_update"); //just in case
		launch_payload("payload.bin");
	}
}

void fix_emu()
{
	if (!(sd_file_exists("sxos/eMMC/00") & sd_file_exists("sxos/eMMC/boot0") & sd_file_exists("sxos/eMMC/boot1")))
		return;

	printerCU("Fix Emu Transfer", "CleanUP...", 0);
	f_mkdir("emuMMC");
	f_mkdir("emuMMC/EF00");
	f_rename("/sxos/eMMC", "/emuMMC/EF00/eMMC");
	f_rename("/Emutendo", "/emuMMC/EF00/Nintendo");

	if (!(sd_file_exists("emuMMC/EF00/eMMC/00") & sd_file_exists("emuMMC/EF00/eMMC/boot0") & sd_file_exists("emuMMC/EF00/eMMC/boot1")))
		return;

	f_unlink("emummc/emummc.bak");
	f_rename("emummc/emummc.ini", "emummc/emummc.bak");
	f_mkdir("emummc");
	FIL fp;
	f_open(&fp, "emummc/emummc.ini", FA_WRITE | FA_CREATE_ALWAYS);
	f_puts("[emummc]\n", &fp);
	f_puts("enabled=1\n", &fp);
	f_puts("path=emuMMC/EF00\n", &fp);
	f_puts("nintendo_path=emuMMC/EF00/Nintendo\n", &fp);
	f_close(&fp);
	sd_save_to_file("", 0, "emuMMC/EF00/file_based");
}