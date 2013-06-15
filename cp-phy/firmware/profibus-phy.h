#ifndef PROFIBUS_PHY_H_
#define PROFIBUS_PHY_H_

#include "util.h"

#include <stdint.h>


enum pb_phy_baud {
	PB_PHY_BAUD_9600,
	PB_PHY_BAUD_19200,
	PB_PHY_BAUD_45450,
	PB_PHY_BAUD_93750,
	PB_PHY_BAUD_187500,
	PB_PHY_BAUD_500000,
	PB_PHY_BAUD_1500000,
	PB_PHY_BAUD_3000000,
	PB_PHY_BAUD_6000000,
	PB_PHY_BAUD_12000000,
};

enum pb_sd {
	PB_SD1	= 0x10,
	PB_SD2	= 0x68,
	PB_SD3	= 0xA2,
	PB_SD4	= 0xDC,
	PB_SC	= 0xE5,
};

enum pb_ed {
	PB_ED	= 0x16,
};

struct pb_telegram {
	union {
		struct pb_telegram_variable_length {
			uint8_t sd_a;
			uint8_t le;
			uint8_t ler;
			uint8_t sd_b;
			uint8_t da;
			uint8_t sa;
			uint8_t fc;
			uint8_t data[246 + 1 + 1]; /* data + fcs + ed */
		} _packed var;

		struct pb_telegram_static_8_bytes {
			uint8_t sd;
			uint8_t da;
			uint8_t sa;
			uint8_t fc;
			uint8_t data[8];
			uint8_t fcs;
			uint8_t ed;
		} _packed stat8;

		struct pb_telegram_static_0_bytes {
			uint8_t sd;
			uint8_t da;
			uint8_t sa;
			uint8_t fc;
			uint8_t fcs;
			uint8_t ed;
		} _packed stat0;

		struct pb_telegram_token {
			uint8_t sd;
			uint8_t da;
			uint8_t sa;
		} _packed token;

		struct pb_telegram_ack {
			uint8_t sd;
		} _packed ack;

		uint8_t sd;
	} _packed;
} _packed;

int8_t pb_sdr(struct pb_telegram *t);
int8_t pb_sdn(struct pb_telegram *t);

enum pb_event {
	PB_EV_SDN_COMPLETE,
	PB_EV_SDR_COMPLETE,
	PB_EV_SDR_ERROR,
};

typedef void (*pb_notifier_t)(enum pb_event event);

void pb_set_notifier(pb_notifier_t notifier);

int8_t pb_phy_init(enum pb_phy_baud baudrate);
void pb_phy_exit(void);

#endif /* PROFIBUS_PHY_H_ */