#ifndef ES_HANDLER_
#define ES_HANDLER_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <string.h>
#include <ANN/ANN.h>

void ES_INFO::set_file_info(FILE **file,
		unsigned int file_num,
		unsigned int file_page_size) {
	m_file = file;
	m_file_num = file_num;
	m_file_page_size = file_page_size;
}

void ES_INFO::set_fix_len(unsigned int fix_len) {
	m_fix_len = fix_len;
	m_buff = new char[fix_len+1];
}

int ES_INFO::mem2pt_bin(char* pts,
		int dim, ANNpoint p) {
	unsigned int dataSize =
			sizeof(double)*dim;
	memcpy((char*)p, pts, dataSize);
}

int ES_INFO::str2Pt(char* pts,
		int dim, ANNpoint p) {
	char *pNext;
	int count;

	if (pts == NULL || strlen(pts) == 0) {
		return -1;
	}

	pNext = strtok(pts, " ");

	int i = 0;
	while( pNext != NULL) {
		p[i] = atof(pNext);
		++i;
		if ( i >= dim ) {
			//			printf("get ok\n");
			return 0;
		}
		pNext = strtok(NULL," ");
	}
	return -2;
}

void ES_INFO::set_g_value(ANNpoint es_value) {
	m_value = es_value;
}

void ES_INFO::get_value_by_index(unsigned int index) {
	switch (m_mode){
	case 0: {
		unsigned long int offset = index*m_fix_len;
		fseek(m_file[0], offset, SEEK_SET);
		fread(m_buff, m_fix_len, 1, m_file[0]);
		m_buff[m_fix_len] = 0;
		//	printf("get value %d\n", index);
		str2Pt(m_buff, m_dim, m_value);
		break;
	}
	case 3: {
		unsigned long int offset =
				index*sizeof(double)*m_dim;
		char *pts = m_mem + offset;
		mem2pt_bin(pts, m_dim, m_value);

//		printf("get index: %d\n", index);
//		for( unsigned int i=0; i<m_dim; ++i ) {
//			printf("%lf ", m_value[i]);
//		}
//		printf("\n");

		break;
	}
	}
}

#endif /* ES_HANDLER_ */
