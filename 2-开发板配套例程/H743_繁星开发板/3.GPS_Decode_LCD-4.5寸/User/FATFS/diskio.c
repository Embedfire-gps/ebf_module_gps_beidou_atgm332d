/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"			/* FatFs lower layer API */
#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "ff.h"

extern USBH_HandleTypeDef hUsbHostFS;
#define USB		    0
#define SECTOR_SIZE		512
static DWORD scratch[_MAX_SS / 4];

PARTITION VolToPart[]=
{
	{0,1},/* "0:" */
	{0,2},/* "1:" */
	{0,3}	/* "2:" */
};

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
  return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
  DRESULT res = RES_ERROR;

  if(USBH_MSC_UnitIsReady(&hUsbHostFS, 0))
  {
    res = RES_OK;
  }
  else
  {
    res = RES_ERROR;
  }

	return res;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;
  USBH_StatusTypeDef  status = USBH_OK;

  if (((DWORD)buff & 3) && (((HCD_HandleTypeDef *)hUsbHostFS.pData)->Init.dma_enable))
  {
    while ((count--)&&(status == USBH_OK))
    {
      status = USBH_MSC_Read(&hUsbHostFS, 0, sector + count, (uint8_t *)scratch, 1);

      if(status == USBH_OK)
      {
        memcpy (&buff[count * _MAX_SS] ,scratch, _MAX_SS);
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    status = USBH_MSC_Read(&hUsbHostFS, 0, sector, buff, count);
  }

  if(status == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&hUsbHostFS, 0, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog ("USB Disk is not ready!");
      res = RES_NOTRDY;
      break;

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;
  USBH_StatusTypeDef  status = USBH_OK;

  if (((DWORD)buff & 3) && (((HCD_HandleTypeDef *)hUsbHostFS.pData)->Init.dma_enable))
  {

    while (count--)
    {
      memcpy (scratch, &buff[count * _MAX_SS], _MAX_SS);

      status = USBH_MSC_Write(&hUsbHostFS, 0, sector + count, (BYTE *)scratch, 1) ;
      if(status == USBH_FAIL)
      {
        break;
      }
    }
  }
  else
  {
    status = USBH_MSC_Write(&hUsbHostFS, 0, sector, (BYTE *)buff, count);
  }

  if(status == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&hUsbHostFS, 0, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_WRITE_PROTECTED:
      USBH_ErrLog("USB Disk is Write protected!");
      res = RES_WRPRT;
      break;

    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog("USB Disk is not ready!");
      res = RES_NOTRDY;
      break;

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
#define MSC_Handle ((MSC_HandleTypeDef *)(hUsbHostFS.pActiveClass->pData))
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
  MSC_LUNTypeDef info;

			//if (drv) return RES_PARERR;
			res = RES_ERROR;

			//if (Stat & STA_NOINIT) return RES_NOTRDY;
			switch (cmd)
			{
				case CTRL_SYNC :		/* Make sure that no pending write process */
					res = RES_OK;
					break;

				case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
					if(USBH_MSC_GetLUNInfo(&hUsbHostFS, 0, &info) == USBH_OK)
					{
						*(DWORD*)buff = info.capacity.block_nbr;
						res = RES_OK;
					}
					else
					{
						res = RES_ERROR;
					}
					break;
				case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
					if(USBH_MSC_GetLUNInfo(&hUsbHostFS, 0, &info) == USBH_OK)
					{
						*(DWORD*)buff = info.capacity.block_size;
						res = RES_OK;
					}
					else
					{
						res = RES_ERROR;
					}
					break;

				case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */\
					if(USBH_MSC_GetLUNInfo(&hUsbHostFS, 0, &info) == USBH_OK)
					{
						*(DWORD*)buff = info.capacity.block_size;
						res = RES_OK;
					}
					else
					{
						res = RES_ERROR;
					}
					break;

				default:
					res = RES_PARERR;
					break;
			}
			return res;
}
#endif

/*
*********************************************************************************************************
*	函 数 名: get_fattime
*	功能说明: 获得系统时间，用于改写文件的创建和修改时间。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
__weak DWORD get_fattime(void) {
	/* Returns current time packed into a DWORD variable */
	return	  ((DWORD)(2013 - 1980) << 25)	/* Year 2013 */
			| ((DWORD)7 << 21)				/* Month 7 */
			| ((DWORD)28 << 16)				/* Mday 28 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				/* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}

