import React from 'react';
import styled from 'styled-components';

const SectionHeader = styled.div.attrs({
    className: "ms-bgColor-themeDarkAlt ms-fontColor-white"
})`

    margin-bottom: 3rem;
    padding: 2rem 2rem 1rem;
    
    h1 {
        margin-top: 0;
        margin-bottom: 0;
        font-weight: 100;
    }
`;

export default SectionHeader;