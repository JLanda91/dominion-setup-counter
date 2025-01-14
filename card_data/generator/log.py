import logging.config

_code_generator_log_config = {
    "version": 1,
    "formatters": {
        "simple": {
            "format": "%(message)s"
        }
    },
    "handlers": {
        "stdout": {
            "class": "logging.StreamHandler",
            "formatter": "simple",
            "stream": "ext://sys.stdout"
        }
    },
    "loggers": {
        "root": {
            "level": "INFO",
            "handlers": [
                "stdout"
            ]
        }
    }
}

logging.config.dictConfig(config=_code_generator_log_config)

code_generator_log = logging.getLogger("code_generator")